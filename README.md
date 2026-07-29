# Espie-AI-LLM-SGM-runs-on-1kb-

A small AI SGM model meant for small ESP32 Devices, works like a modern LLM but natively for an ESP32

# What is Espie?

Espie is a small AI SGM where it is like a AI LLM but runs completely locally on a ESP32 with no api or external files 

# Why this matters?

Because Since Espie runs on a literal microcontroller, this proves that modern AI doesnt need

-GPU clusters

-Water cooling

-Terabytes of Ram

-20 megawatts of serious power

because Espie runs on

-1kb of ram (16 is allocated as a buffer but 1kb is estimated usage)

-No water cooling

-usually Mah Or sips on small watts per hour

-512/520 kb of SRAM and sometimes just an extra 8MB of PSRAM (sometimes may be 4mb or 2mb but will vary)

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

Step 1: First download the bin file named "espie_1_6_v2.bin"

Step 2: Copy the ino file into arduino ide

Step 3: compile and flash onto a ESP32 chip 

Step 4: go to this site

https://espressif.github.io/esptool-js/

Step 5: There will be a box , change it to 0x3A0000 , like in the image


<img width="751" height="453" alt="image" src="https://github.com/user-attachments/assets/67a7fd2a-9058-4484-b140-4d36dcb09163" />

Step 6: Once you have it ready, click on "Choose File" and select the bin file you just downloaded

Step 7: Click the big blue button that says "PROGRAM" to flash it

Step 8: after it finished, press the RST button on your ESP32 and head to serial monitor and press it Again

Step 9: you should be able to start typing and then it would respond back!



# Notes

Espie needs

ESP32

No cooling

and its extremely efficient!



# Credits to

-Elias, my best friend, idk what he did even to this project but idk i just calling him out

# A few things to keep in direct mind

I am sorry if its not enough at all, because i am 12 and like i am busy for like....my entire day because i am releasing a new OS soon so like i am sorry if it aint enough, but if you got anything to say, go ahead! go and say it out!
also i will make a few files that are named
"NOTES.txt" that will contain my notes for the full thing on a AI SGM so thanks for cooperating guys (or whoever is reading this)

# Files and stuff you need if you dont need Bloat

Espie.ino

Espie_1_6_v2.bin

Instructions.txt

License   (read this please!)

# Extra Files that aren't needed (optional, but read them)

Readme





GO NUTS HEHEHAHAHAHAHAHAHHAH YOU HAVE SOME NEW TECH TO FORK AND BLAH BLAH BLAH
