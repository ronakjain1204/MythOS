#include "include/idt.h"

extern "C" void load_idt(IDTPointer*);

IDTEntry idt[256];

void set_idt_entry(int vector, uint32_t isr, uint16_t selector, uint8_t flags) {
    idt[vector].offset_low = isr & 0xFFFF;
    idt[vector].selector = selector;
    idt[vector].zero = 0;
    idt[vector].type_attr = flags;
    idt[vector].offset_high = (isr >> 16) & 0xFFFF;
}

void init_idt() {
    IDTPointer idtp;
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;

    load_idt(&idtp);
}
