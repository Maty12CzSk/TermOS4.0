ENTRY(_start)

SECTIONS
{
    . = 1M;  /* 0x00100000, Multiboot default load address */

    .multiboot : { *(.multiboot) }
    .text : { *(.text*) }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) *(COMMON) }
}