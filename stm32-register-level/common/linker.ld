/* Linker script for the STM32F411RE: 512 KB flash at 0x08000000, 128 KB RAM at 0x20000000. */
ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);   /* stack grows down from the top of RAM */

SECTIONS
{
    .isr_vector : { KEEP(*(.isr_vector)) } > FLASH   /* must be first: the core reads it at reset */
    .text       : { *(.text*) *(.rodata*) . = ALIGN(4); } > FLASH

    _sidata = LOADADDR(.data);                       /* where the initial values of .data sit in flash */
    .data : { _sdata = .; *(.data*) . = ALIGN(4); _edata = .; } > RAM AT > FLASH
    .bss  : { _sbss = .; *(.bss*) *(COMMON) . = ALIGN(4); _ebss = .; } > RAM

    /DISCARD/ : { *(.ARM.exidx*) *(.comment) }
}
