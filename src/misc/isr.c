#include "types.h"
#include "kstdio.h"
#include "kprintf.h"

#define KBD_BUFFER_SIZE 64

typedef struct {
    uint32_t edi, esi, ebp, esp;
    uint32_t ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

static const char *exceptions[] = {
    "Divide by Zero", "Debug", "NMI", "Breakpoint",
    "Overflow", "Bound Range Exceeded", "Invalid Opcode",
    "Device Not Available", "Double Fault",
    "Coprocessor Segment Overrun", "Invalid TSS",
    "Segment Not Present", "Stack Fault",
    "General Protection Fault", "Page Fault", "Reserved",
    "x87 FPU Error", "Alignment Check",
    "Machine Check", "SIMD FPU Error"
};

static uint8_t kbd_buffer[KBD_BUFFER_SIZE];
static uint8_t kbd_head = 0;
static uint8_t kbd_tail = 0;

void isr_handler(registers_t *regs)
{
    const char *name = (regs->int_no < 20)
                       ? exceptions[regs->int_no]
                       : "Reserved";
    kprintf(0, 0x4F, "EXCEPTION %u: %s", regs->int_no, name);
    kprintf(1, 0x4F, "EIP: %X  ERR: %X", regs->eip, regs->err_code);
    __asm__ volatile ("cli; hlt");
}

void irq_handler(registers_t *regs)
{
    if (regs->int_no == 33)
    {
        uint8_t scancode = inb(0x60);
        if (!(scancode & 0x80))
        {
            uint8_t next = (kbd_head + 1) % KBD_BUFFER_SIZE;
            if (next != kbd_tail)
            {
                kbd_buffer[kbd_head] = scancode;
                kbd_head = next;
            }
        }
    }
    if (regs->int_no >= 40)
        port_byte_out(0xA0, 0x20);
    port_byte_out(0x20, 0x20);
}