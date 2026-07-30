# Espie-AI-SGM-runs-on-16kb-

A small AI SGM model meant for small ESP32 Devices, Conceptually works like a LLM, but natively for an ESP32

# What is Espie?

Espie is a small AI SGM where it is like a AI but runs completely locally on a ESP32 with no api or external files (meaning no Openai, groq or any of that is used at all)

# Why this matters?

Because Since Espie runs on a literal microcontroller, this proves that modern AI doesnt need

-GPU clusters

-Water cooling

-Terabytes of Ram

-20 megawatts of serious power

because Espie runs on

-16 KB SRAM Arena and actual 1.4kb being used

-No water cooling

-usually Mah Or sips on small watts per hour

-512/520 kb of SRAM and sometimes just an extra 8MB of PSRAM (sometimes may be 4mb or 2mb but will vary)

AND This demonstrates that an AI text-generation system can run locally on a microcontroller with a very small SRAM working set as low as......16kb...
# What is Espie made out of?

Espie is a AI SGM, not a true llm at all, 

and AI SGM means

Artifical Intelligence Script Generator Model

which means this consists of

-Script/Logic

-Generator Output and Input parser

-Dataset that the Script/Logic references 

meaning this is wayy smaller.

the only thing what makes Espie similar to an LLM , is that it just uses temperature sampling but other than that it is fundamentally different



# How to setup Espie

Step 1: Train Espie by downloading the py and make sure having PyTorch installed

Step 2: Copy the ino file into arduino ide

Step 3: compile and flash onto a ESP32 chip 

Step 4: wait until its trained, when its done it should spit out a file named "Espie_1_6_v2.bin" then go to this site

https://espressif.github.io/esptool-js/

Step 5: There will be a box , change it to 0x3A0000 , like in the image


<img width="751" height="453" alt="image" src="https://github.com/user-attachments/assets/67a7fd2a-9058-4484-b140-4d36dcb09163" />

Step 6: Once you have it ready, click on "Choose File" and select the bin file you just got from training Espie

Step 7: Click the big blue button that says "PROGRAM" to flash it

Step 8: after it finished, press the RST button on your ESP32 and head to serial monitor and press it Again

Step 9: you should be able to start typing and then it would respond back!



# Notes

Espie needs

ESP32

No cooling

and its extremely efficient!

# How Espie actually works

internally its
Input
 ↓
Parser
 ↓
Intent / context
 ↓
Script selection/generation
 ↓
Small model inference
 ↓
Sampling
 ↓
Output  (I HATE COPY AND PASTING ↓ )

# Credits to

-Elias,Joel, my best friends, idk what they did even to this project but idk i just calling him out but i did everything in this project


# A few things to keep in direct mind

I am sorry if its not enough at all, because i am 12 and like i am busy for like....my entire day because i am releasing a new OS soon so like i am sorry if it aint enough, but if you got anything to say, go ahead! go and say it out!
also i will make a few files that are named
"NOTES.txt" that will contain my notes for the full thing on a AI SGM so thanks for cooperating guys (or whoever is reading this)


# Benchmarks and some actual technical info

ESP32-S3

Model flash:       1.6 MB


Peak SRAM:          1.4 KB

Context:            256 bytes

reserved SRAM: 16KB 

Compiler/toolchain: Arduino IDE as the IDE i used, then using Espressif's ESP32 core 2.0.11

Tokens Per Second (TPS):  15 to 30 tokens per second atleast

flash consumption: the entire thing (ino plus the trained bin) is around 1.7~ Mb

Reserved arena: 16 KB

Measured working usage: ~1.4 KB


Average generation: May vary on ESP32 chip
and a note again, it says 16kb in the code but if you actually see the runtime (thats not counting arduino ide's extra bloat such as FreeRTOS, and a few other stuff) then its 
1.4KB of SRAM notes
the direct ESP32 model i used for this test was     LILY GO T DISPLAY S3 

# Files and stuff you need if you dont need Bloat

Espie.ino

param.py (to teach espie to get the bin

Instructions.txt

License   (read this please!)

# Extra Files that aren't needed (optional, but read them)

Readme





GO NUTS HEHEHAHAHAHAHAHAHHAH YOU HAVE SOME NEW TECH TO FORK (with my permission) AND BLAH BLAH BLAH
