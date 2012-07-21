PREFIX = arm-none-eabi

CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
CP      = arm-none-eabi-objcopy
OD      = arm-none-eabi-objdump

TOOLCHAIN_DIR := $(shell dirname `which $(CC)`)/../$(PREFIX)

CFLAGS		+= -Os -g -Wall -Wextra -I$(TOOLCHAIN_DIR)/include \
		   -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -DSTM32F1
LDFLAGS		+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group \
		   -nostartfiles -Wl,--gc-sections \
			-L$(TOOLCHAIN_DIR)/lib -L$(TOOLCHAIN_DIR)/lib/stm32/f1 \
		   -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float
all: test

clean:
	rm -f *.lst *.o *.elf *.bin

test:main.bin

main.bin: main.elf
	@ echo "[Copying]"
	$(CP) -Obinary  main.elf main.bin
	$(OD) -S main.elf > main.lst

main.elf: main.o
	@ echo "[Linking]"
	$(LD) -T stm32vl-discovery.ld $(LDFLAGS) main.o -lopencm3_stm32f1 -o main.elf

main.o: main.c
	@ echo "[Compiling main code]"
	$(CC) -g -Wall -pedantic -I./ -c $(CFLAGS) main.c -Os

prog: test
	sudo st-flash write v1 main.bin 0x08000000


