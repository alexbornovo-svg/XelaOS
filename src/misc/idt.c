#include "idt.h"
#include "kstdio.h"
#include "kprintf.h"
#include "kbd.h"

static idt_entry_t idt[256];
static idt_ptr_t idt_ptr;

extern void irq0(void);  extern void irq1(void);  extern void irq2(void);
extern void irq3(void);  extern void irq4(void);  extern void irq5(void);
extern void irq6(void);  extern void irq7(void);  extern void irq8(void);
extern void irq9(void);  extern void irq10(void); extern void irq11(void);
extern void irq12(void); extern void irq13(void); extern void irq14(void);
extern void irq15(void);

void idt_set_gate(uint8_t n, uint32_t handler, uint16_t selector, uint8_t flags)
{
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
    idt[n].selector = selector;
    idt[n].zero = 0;
    idt[n].flags = flags;
}

void idt_init()
{
    __asm__ volatile ("cli");

    idt_ptr.limit = (sizeof(idt_entry_t) * 256) - 1;
    idt_ptr.base  = (uint32_t)&idt;

    uint8_t *p = (uint8_t *)idt;
    uint32_t i;
    for (i = 0; i < sizeof(idt); i++) p[i] = 0;

    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01); 
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0xFF); 
    port_byte_out(0xA1, 0xFF);

    idt_set_gate(0,  (uint32_t)isr0,  0x08, 0x8E);
    kprintf(0, WHITE, "0");
    idt_set_gate(1,  (uint32_t)isr1,  0x08, 0x8E);
    kprintf(0, WHITE, "1");
    idt_set_gate(2,  (uint32_t)isr2,  0x08, 0x8E);
    kprintf(0, WHITE, "2");
    idt_set_gate(3,  (uint32_t)isr3,  0x08, 0x8E);
    kprintf(0, WHITE, "3");
    idt_set_gate(4,  (uint32_t)isr4,  0x08, 0x8E);
    kprintf(0, WHITE, "4");
    idt_set_gate(5,  (uint32_t)isr5,  0x08, 0x8E);
    kprintf(0, WHITE, "5");
    idt_set_gate(6,  (uint32_t)isr6,  0x08, 0x8E);
    kprintf(0, WHITE, "6");
    idt_set_gate(7,  (uint32_t)isr7,  0x08, 0x8E);
    kprintf(0, WHITE, "7");
    idt_set_gate(8,  (uint32_t)isr8,  0x08, 0x8E);
    kprintf(0, WHITE, "8");
    idt_set_gate(9,  (uint32_t)isr9,  0x08, 0x8E);
    kprintf(0, WHITE, "9");
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    kprintf(0, WHITE, "10");
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    kprintf(0, WHITE, "11");
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    kprintf(0, WHITE, "12");
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    kprintf(0, WHITE, "13");
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    kprintf(0, WHITE, "14");
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    kprintf(0, WHITE, "15");
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    kprintf(0, WHITE, "16");
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    kprintf(0, WHITE, "17");
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    kprintf(0, WHITE, "18");
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    kprintf(0, WHITE, "19");
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    kprintf(0, WHITE, "20");
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    kprintf(0, WHITE, "21");
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    kprintf(0, WHITE, "22");
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    kprintf(0, WHITE, "23");
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    kprintf(0, WHITE, "24");
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    kprintf(0, WHITE, "25");
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    kprintf(0, WHITE, "26");
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    kprintf(0, WHITE, "27");
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    kprintf(0, WHITE, "28");
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    kprintf(0, WHITE, "29");
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    kprintf(0, WHITE, "30");
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);
    kprintf(0, WHITE, "31");
    idt_set_gate(32, (uint32_t)irq0,  0x08, 0x8E);
    kprintf(0, WHITE, "32");
    idt_set_gate(33, (uint32_t)irq1,  0x08, 0x8E);
    kprintf(0, WHITE, "33");
    idt_set_gate(34, (uint32_t)irq2,  0x08, 0x8E);
    kprintf(0, WHITE, "34");
    idt_set_gate(35, (uint32_t)irq3,  0x08, 0x8E);
    kprintf(0, WHITE, "35");
    idt_set_gate(36, (uint32_t)irq4,  0x08, 0x8E);
    kprintf(0, WHITE, "36");
    idt_set_gate(37, (uint32_t)irq5,  0x08, 0x8E);
    kprintf(0, WHITE, "37");
    idt_set_gate(38, (uint32_t)irq6,  0x08, 0x8E);
    kprintf(0, WHITE, "38");
    idt_set_gate(39, (uint32_t)irq7,  0x08, 0x8E);
    kprintf(0, WHITE, "39");
    idt_set_gate(40, (uint32_t)irq8,  0x08, 0x8E);
    kprintf(0, WHITE, "40");
    idt_set_gate(41, (uint32_t)irq9,  0x08, 0x8E);
    kprintf(0, WHITE, "41");
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    kprintf(0, WHITE, "42");
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    kprintf(0, WHITE, "43");
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    kprintf(0, WHITE, "44");
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    kprintf(0, WHITE, "45");
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    kprintf(0, WHITE, "46");
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);
    kprintf(0, WHITE, "47");

    port_byte_out(0x21, 0xFD);
    port_byte_out(0xA1, 0xFF);

    kprintf(23, WHITE, "idt_entry size: %u", sizeof(idt_entry_t));
    kprintf(24, 0x07, "IDT limit: %u  base: %X", idt_ptr.limit, idt_ptr.base);
    kprintf(19, WHITE, "IDT addr: %X", &idt);
    kprintf(20, WHITE, "IDT_PTR addr: %X", &idt_ptr);
    kprintf(21, WHITE, "IDT base: %X", idt_ptr.base);
    kprintf(22, WHITE, "IDT limit: %u", idt_ptr.limit);

    __asm__ volatile ("lidt (%0)" : : "r" (&idt_ptr));
}