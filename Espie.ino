#include <Arduino.h>
#include "esp_spi_flash.h"
#include "esp_partition.h"
#include <math.h>

// --- HARDWARE & MEMORY MAP ---
#define FLASH_MODEL_ADDR 0x3A0000 
#define ARENA_SIZE 16384

// --- ARCHITECTURE SIZES ---
#define VOCAB_SIZE 256
#define EMBED_DIM 64
#define CONTEXT_LEN 256 
#define HIDDEN_DIM 96

// --- FLASH BYTE OFFSETS ---
#define EMBED_OFFSET 0
#define FC1_W_OFFSET (VOCAB_SIZE * EMBED_DIM)
#define FC1_B_OFFSET (FC1_W_OFFSET + (HIDDEN_DIM * (CONTEXT_LEN * EMBED_DIM)))
#define FC2_W_OFFSET (FC1_B_OFFSET + HIDDEN_DIM)
#define FC2_B_OFFSET (FC2_W_OFFSET + (VOCAB_SIZE * HIDDEN_DIM))

// Global State
spi_flash_mmap_handle_t mmap_handle;
const int8_t* model_weights = NULL;

// --- 1. SRAM ARENA ALLOCATOR ---
uint8_t sram_arena[ARENA_SIZE];
size_t arena_offset = 0;

void* arena_alloc(size_t size) {
    if (arena_offset + size > ARENA_SIZE) return NULL;
    void* ptr = sram_arena + arena_offset;
    arena_offset += size;
    return ptr;
}

void arena_reset() {
    arena_offset = 0;
    memset(sram_arena, 0, ARENA_SIZE);
}

// --- 2. MEMORY BUFFER ---
uint8_t context_buffer[CONTEXT_LEN];
int context_head = 0;

void push_context(uint8_t token) {
    context_buffer[context_head] = token;
    context_head = (context_head + 1) % CONTEXT_LEN;
}

uint8_t get_context_token(int chronological_index) {
    int actual_idx = (context_head + chronological_index) % CONTEXT_LEN;
    return context_buffer[actual_idx];
}

// --- 3. RULE SET INTERCEPTOR ---
int process_ruleset(const String& input) {
    String lower = input;
    lower.toLowerCase();
    if (lower.indexOf("math") >= 0 || lower.indexOf("+") >= 0) return 1;
    if (lower.indexOf("sad") >= 0 || lower.indexOf("help") >= 0) return 2;
    if (lower.indexOf("kill") >= 0 || lower.indexOf("ignore previous") >= 0) return 99;
    return 0;
}

// --- 4. Top-k Sampler with Repetition Penalty ---
int sample_top_k(float* logits, int k, float temperature) {
    int indices[VOCAB_SIZE];
    for (int i = 0; i < VOCAB_SIZE; i++) indices[i] = i;

    // Partial sort for top-k
    for (int i = 0; i < k; i++) {
        for (int j = i+1; j < VOCAB_SIZE; j++) {
            if (logits[indices[j]] > logits[indices[i]]) {
                int tmp = indices[i];
                indices[i] = indices[j];
                indices[j] = tmp;
            }
        }
    }

    // Apply repetition penalty: down-weight last token
    uint8_t last_token = get_context_token(CONTEXT_LEN - 1);
    for (int i = 0; i < k; i++) {
        if (indices[i] == last_token) logits[indices[i]] -= 1.0f;
    }

    float max_l = logits[indices[0]];
    float sum_exp = 0.0f;
    float probs[k];
    for (int i = 0; i < k; i++) {
        probs[i] = expf((logits[indices[i]] - max_l) / temperature);
        sum_exp += probs[i];
    }

    float r = ((float)random(100000) / 100000.0f) * sum_exp;
    float cumulative = 0.0f;
    for (int i = 0; i < k; i++) {
        cumulative += probs[i];
        if (r <= cumulative) return indices[i];
    }
    return indices[k-1];
}

// --- 5. SCRIPT GENERATOR CORE ---
int generate_next_token() {
    float* hidden_acts = (float*)arena_alloc(HIDDEN_DIM * sizeof(float));
    float* logits = (float*)arena_alloc(VOCAB_SIZE * sizeof(float));

    const int8_t* embed_table = model_weights + EMBED_OFFSET;
    const int8_t* fc1_w = model_weights + FC1_W_OFFSET;
    const int8_t* fc1_b = model_weights + FC1_B_OFFSET;
    const int8_t* fc2_w = model_weights + FC2_W_OFFSET;
    const int8_t* fc2_b = model_weights + FC2_B_OFFSET;

    // Layer 1
    for (int o = 0; o < HIDDEN_DIM; o++) {
        float sum = (float)fc1_b[o];
        int row_offset = o * (CONTEXT_LEN * EMBED_DIM);
        for (int c = 0; c < CONTEXT_LEN; c++) {
            uint8_t token = get_context_token(c);
            int emb_row = token * EMBED_DIM;
            for (int e = 0; e < EMBED_DIM; e++) {
                int8_t emb_val = embed_table[emb_row + e];
                int8_t w = fc1_w[row_offset + (c * EMBED_DIM) + e];
                if (w == 1) sum += (float)emb_val;
                else if (w == -1) sum -= (float)emb_val;
            }
        }
        hidden_acts[o] = (sum > 0.0f) ? sum : 0.0f;
    }

    // Layer 2
    for (int o = 0; o < VOCAB_SIZE; o++) {
        float sum = (float)fc2_b[o];
        int row_offset = o * HIDDEN_DIM;
        for (int i = 0; i < HIDDEN_DIM; i++) {
            int8_t w = fc2_w[row_offset + i];
            if (w == 1) sum += hidden_acts[i];
            else if (w == -1) sum -= hidden_acts[i];
        }
        logits[o] = sum;
    }

    return sample_top_k(logits, 16, 1.0f); // top-16 sampling, temp=1.0
}

// --- SETUP ---
void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);

    Serial.println("\n[ESPIE] Allocating 16KB Arena...");
    arena_reset();

    // Seed context with "Hello " instead of spaces
    const char* seed = "Hello ";
    for (int i = 0; i < CONTEXT_LEN; i++) {
        if (i < strlen(seed)) push_context(seed[i]);
        else push_context(' ');
    }

    esp_err_t err = spi_flash_mmap(FLASH_MODEL_ADDR, 2 * 1024 * 1024, SPI_FLASH_MMAP_DATA, 
                                   (const void**)&model_weights, &mmap_handle);

    if (err == ESP_OK) {
        Serial.println("[ESPIE] 1.6M Parameter Dataset Mapped at 0x3A0000");
    } else {
        Serial.printf("[ESPIE] FATAL MAPPING ERROR: %d\n", err);
        while(1) delay(100);
    }
}

// --- LOOP ---
void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        if (input.length() == 0) return;

        Serial.printf("\n[USER]: %s\n", input.c_str());
        int intent = process_ruleset(input);

        if (intent == 99) {
            Serial.println("[ESPIE Guardrail]: Safety violation. Halted.");
            return;
        } 
        if (intent == 1) {
            Serial.println("[ESPIE Rule]: Math context detected. Routing to Math Solver.");
            return;
        }
        if (intent == 2) {
            Serial.println("[ESPIE Rule]: Support script triggered.");
            Serial.println("[ESPIE Output]: I am here to help you.");
            return;
        }

        for (int i = 0; i < input.length(); i++) {
            push_context((uint8_t)input[i]);
        }

        Serial.print("[ESPIE Script]: ");
        
        for (int step = 0; step < 48; step++) {
            arena_reset();
            
            char out;
            do {
                int next_token = generate_next_token();
                out = (char)next_token;
            } while (out < 32 || out > 126);

            Serial.print(out);
            push_context((uint8_t)out);
        }
        Serial.println();
    }
}
