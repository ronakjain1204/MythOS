gdt_start:
    dq 0x0000000000000000     ; Null descriptor (must be zero)
    dq 0x00CF9A000000FFFF     ; Code segment descriptor
    dq 0x00CF92000000FFFF     ; Data segment descriptor
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size of the GDT - 1
    dd gdt_start                ; Address of GDT
