#include "gdt.h"

static gdt_entry_t gdt[3];
static gdt_ptr_t   gdt_ptr;

static void gdt_set_gate(int n, uint32_t base, uint32_t limit,
                          uint8_t access, uint8_t gran)
{
    gdt[n].base_low  = base & 0xFFFF;
    gdt[n].base_mid  = (base >> 16) & 0xFF;
    gdt[n].base_high = (base >> 24) & 0xFF;
    gdt[n].limit_low    = limit & 0xFFFF;
    gdt[n].granularity  = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[n].access       = access;
}

void gdt_init(void)
{
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 3) - 1;
    gdt_ptr.base  = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0,          0x00, 0x00); // null
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // code  selector 0x08
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // data  selector 0x10

    __asm__ volatile (
        "lgdt (%0)\n"
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        "ljmp $0x08, $1f\n"   // far jump per ricaricare CS
        "1:\n"
        : : "r"(&gdt_ptr) : "eax"
    );
}