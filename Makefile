TARGET = level
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m4 -mthumb -Wall -ffreestanding -nostdlib -T linker.ld

all: $(TARGET).bin

$(TARGET).elf: src/startup.c src/main.c linker.ld
	$(CC) $(CFLAGS) src/startup.c src/main.c -o $(TARGET).elf

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

flash: $(TARGET).bin
	cp $(TARGET).bin /mnt/d/

clean:
	rm -f $(TARGET).elf $(TARGET).bin