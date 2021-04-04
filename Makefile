TARGET  = bluepill
ARCH    = arm-none-eabi
CFLAGS  = -W -Wall -g -Os -mcpu=cortex-m3 -mthumb -mabi=aapcs -mfloat-abi=soft $(CFLAGS_EXTRA)
LDFLAGS = -Wl,--gc-sections -march=armv7-m -mabi=aapcs  -nostartfiles -nostdlib -lc -lnosys -lgcc
AFLAGS  = --warn --fatal-warnings -mcpu=cortex-m3
OBJS    = obj/bootstrap.o obj/main.o

all: $(TARGET).hex

$(TARGET).bin: obj/$(TARGET).elf
	$(ARCH)-objcopy -O binary $< $@

$(TARGET).hex: $(TARGET).bin
	$(ARCH)-objcopy -I binary -O ihex --change-address 0x8000000 $< $@

obj/$(TARGET).elf: $(OBJS)
	$(ARCH)-gcc $^ $(LDFLAGS) -T link.ld -o $@ -Wl,-Map=$@.map

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(ARCH)-gcc $(CFLAGS) -c $< -fdata-sections -ffunction-sections -o $@

obj/%.o: %.s
	@mkdir -p $(dir $@)
	$(ARCH)-as $(AFLAGS) $< -o $@

clean:
	@rm -rf *.{bin,elf,map,lst,tgz,zip,hex} obj
