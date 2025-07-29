; Multiboot header
section .multiboot
align 4
    dd 0x1BADB002              ; magic
    dd 0x0                     ; flags
    dd -(0x1BADB002 + 0x0)     ; checksum

section .text
global _start
extern kernel_main

_start:
    mov esp, stack_top
    call kernel_main
    cli

.hang:
    hlt
    jmp .hang

section .bss
align 4
stack:
    resb 16384                 ; 16 KB stack
stack_top: