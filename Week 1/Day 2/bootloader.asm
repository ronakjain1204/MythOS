[BITS 16]     ; Tell assembler we are writing 16-bit code (Real Mode)
[ORG 0x7C00]  ; BIOS loads the bootloader at memory address 0x7C00

start:
    mov si, message     ; Load address of message into SI register
    call print_string   ; Call the function to print the message
    jmp $              ; Infinite loop to stop execution

print_string:
    lodsb              ; Load next character from SI into AL
    or al, al          ; Check if AL is zero (end of string)
    jz done            ; If zero, we are done
    mov ah, 0x0E       ; BIOS teletype function (prints a character)
    int 0x10           ; Call BIOS interrupt
    jmp print_string   ; Repeat for next character
done:
    ret

message db "Hello, MythOS!", 0  ; Define the message to print

times 510 - ($ - $$) db 0  ; Fill the rest of the boot sector with zeroes
dw 0xAA55  ; Boot sector signature (BIOS requires this to boot)
