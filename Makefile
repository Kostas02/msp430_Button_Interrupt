# makfile configuration
NAME            = msp430_Button_Interrupt
OBJECTS         = main.o
CPU             = msp430g2452

CFLAGS          = -mmcu=${CPU} -O2 -Wall -g

#switch the compiler (for the internal make rules)
CC              = msp430-gcc


.PHONY: all FORCE clean download dist

#all should be the first target. it's built when make is run without args
all: ${NAME}.elf ${NAME}.a43 ${NAME}.lst

#confgigure the next line if you want to use the serial download
download: flash

#additional rules for files
${NAME}.elf: ${OBJECTS}
	${CC} -mmcu=${CPU} -o $@ ${OBJECTS}

${NAME}.a43: ${NAME}.elf
	msp430-objcopy -O ihex $^ $@

${NAME}.lst: ${NAME}.elf
	msp430-objdump -dSt $^ >$@

#Added to program flash with mspdebug
flash: ${NAME}.elf
	mspdebug rf2500 "prog ${NAME}.elf"

flasherase:
	mspdebug rf2500 erase

clean:
	rm -f ${NAME}.elf ${NAME}.a43 ${NAME}.lst ${OBJECTS}

#dummy target as dependecy if something has to be build everytime
FORCE:


#project dependencies
main.o: main.c

