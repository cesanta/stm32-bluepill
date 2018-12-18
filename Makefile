TARGET	= fw
ARCH	= arm-none-eabi
CFLAGS	= -W -Wall -O2 -g -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0 -mthumb
AFLAGS	= --warn --fatal-warnings -mcpu=cortex-m0
OBJS	= obj/bootstrap.o obj/main.o

all: $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	$(ARCH)-objcopy -O binary $< $@

$(TARGET).elf: $(OBJS)
	$(ARCH)-ld $^ -T link.ld -o $@
# $(ARCH)-objdump -D $@ > $(TARGET).lst

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(ARCH)-gcc $(CFLAGS) -c $< -o $@

obj/%.o: %.s
	@mkdir -p $(dir $@)
	$(ARCH)-as $(AFLAGS) $< -o $@

COM_PORT ?= /dev/cu.SLAB_USBtoUART
flash: $(TARGET).bin
	python stm32loader.py -p $(COM_PORT) -b 9600 -Vewv $(TARGET).bin

clean:
	@rm -rf *.{bin,elf,map,lst} obj *.tgz
