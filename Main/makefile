PART=atmega8

compile: main.hex

all: compile flash

compile: main.hex

flash: main.hex
	avrdude -p $(PART) -c stk500 -U flash:w:$^:i -P /dev/ttyACM0 #avrdoper
verify: main.hex
	avrdude -p $(PART) -c stk500 -U flash:v:$^:i -P /dev/ttyACM0 #avrdoper

main.hex: *.c
	avr-gcc -mmcu=$(PART) -o main.elf -O2 $^
	avr-objcopy -O ihex main.elf $@

clean: 
	rm *.{hex,elf,eep} 2>/dev/null ||true


.PHONY: compile flash verify clean
