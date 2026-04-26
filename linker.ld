ENTRY(loader)
SECTIONS {
    . = 1M;

    .text ALIGN(4096) : {
        *(.multiboot)
        *(.text*)
    }

    .data ALIGN(4096) : {
        *(.data)
        *(.rodata*)
    }

    .bss ALIGN(4096) : {
        *(.bss)
        *(COMMON)
    }
}