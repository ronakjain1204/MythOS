#include "include/idt.h"
#include "include/isr.h"

extern "C" void keyboard_handler();

void isr_install() {
    set_idt_entry(0x21, (uint32_t)keyboard_handler, 0x08, 0x8E);
}
