ENTRY(_reset)
MEMORY {
  rom : ORIGIN = 0x08000000, LENGTH = 0x10000
  ram : ORIGIN = 0x20000000, LENGTH = 0x10000
}
SECTIONS {
  .text : { *(.text*) } > rom
  .rodata : { *(.rodata*) } > rom

  .bss : {
    _bss_start = .;
    *(.bss SORT(.bss.*) COMMON)
    _bss_end = .;
  } > ram

  .data : {
    _data_start = .;
    *(.data SORT(.data.*))
    _data_end = .;
  } > ram AT > rom
  _data_flash_start = LOADADDR(.data);
}
