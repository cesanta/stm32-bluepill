TARGET  = bluepill
ARCH    = arm-none-eabi
CFLAGS  = -W -Wall -O2 -g -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0 -mthumb
AFLAGS  = --warn --fatal-warnings -mcpu=cortex-m0
OBJS    = obj/bootstrap.o obj/main.o

all: $(TARGET).bin

$(TARGET).bin: obj/$(TARGET).elf
	$(ARCH)-objcopy -O binary $< $@

obj/$(TARGET).elf: $(OBJS)
	$(ARCH)-ld $^ -T link.ld -o $@
# $(ARCH)-objdump -D $@ > $(TARGET).lst

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(ARCH)-gcc $(CFLAGS) -c $< -o $@

obj/%.o: %.s
	@mkdir -p $(dir $@)
	$(ARCH)-as $(AFLAGS) $< -o $@

zip: $(TARGET).bin
	mos create-fw-bundle -o $(TARGET).zip --name $(TARGET) --platform ccm_host host_fw:type=STM32,src=$(TARGET).bin,addr=0x8000000

COM_PORT ?= /dev/cu.SLAB_USBtoUART
flash: $(TARGET).bin
	python stm32loader.py -p $(COM_PORT) -Vewv $(TARGET).bin

clean:
	@rm -rf *.{bin,elf,map,lst} obj *.tgz *.zip
