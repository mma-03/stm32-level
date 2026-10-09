TARGET = level
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m4 -mthumb -Wall -ffreestanding -nostdlib -T linker.ld -Iinclude

SRCS = src/startup.c src/main.c src/timer.c src/uart.c src/i2c.c src/mpu6500.c
HDRS = include/stm32f401.h include/timer.h include/uart.h include/i2c.h include/mpu6500.h

all: $(TARGET).bin

$(TARGET).elf: $(SRCS) $(HDRS) linker.ld
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET).elf

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

flash: $(TARGET).bin
	cp $(TARGET).bin /mnt/d/

clean:
	rm -f $(TARGET).elf $(TARGET).bin