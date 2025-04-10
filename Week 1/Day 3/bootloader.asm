[BITS 16]        ; We're in 16-bit Real Mode
[ORG 0x7C00]     ; BIOS loads us here

start:
    cli                     ; Disable interrupts
    lgdt [gdt_descriptor]   ; Load our new GDT

    mov eax, cr0
    or eax, 1               ; Set PE bit (Protected Mode Enable)
    mov cr0, eax

    jmp 0x08:init_pm        ; Far jump to 32-bit code at selector 0x08

[BITS 32]        ; Now we're writing 32-bit code!

init_pm:
    mov ax, 0x10            ; Data segment (0x10 = 2nd GDT entry)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x90000        ; Set stack pointer

    call kernel_main        ; Jump to kernel (will define later)

hang:
    jmp hang                ; Infinite loop

%include "gdt.asm"

kernel_main:
    ret

times 510 - ($ - $$) db 0  ; Pad boot sector to 510 bytes
dw 0xAA55                  ; Boot signature
