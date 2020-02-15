@echo off
"C:\Program Files\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.1.95\AVRToolchain\bin\avr-g++.exe" -o "10 motors master control.elf"  "10 motors master control.o"   -Wl,-Map="10 motors master control.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group  -mmcu=atmega328p  
