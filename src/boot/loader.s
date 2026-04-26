; Multiboot header
MBOOT_MAGIC    equ 0x1BADB002
MBOOT_FLAGS    equ 0x00
MBOOT_CHECKSUM equ -(MBOOT_MAGIC + MBOOT_FLAGS)

section .multiboot
align 4
    dd MBOOT_MAGIC
    dd MBOOT_FLAGS
    dd MBOOT_CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384          ; 16KB stack
stack_top:

section .text
global loader

loader:
    cli
    mov esp, stack_top
    extern k_main
    call k_main
    hlt

section .note.GNU-stack noalloc noexec nowrite progbits