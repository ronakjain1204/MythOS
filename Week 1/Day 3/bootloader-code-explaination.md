## 💾 Full Day 3 `bootloader.asm` (For Reference)

```nasm
[BITS 16]
[ORG 0x7C00]

start:
    cli                     ; Step 1
    lgdt [gdt_descriptor]   ; Step 2

    mov eax, cr0            ; Step 3
    or eax, 1
    mov cr0, eax

    jmp 0x08:init_pm        ; Step 4

[BITS 32]
init_pm:
    mov ax, 0x10            ; Step 5
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x90000        ; Step 6
    call kernel_main        ; Step 7

hang:
    jmp hang                ; Step 8

kernel_main:
    ret

%include "gdt.asm"          ; Step 9

times 510 - ($ - $$) db 0   ; Padding
dw 0xAA55                   ; Boot Signature
```

---

## 🔍 Detailed Explanation, Line by Line

---

### 🧠 `[BITS 16]`  
Tells the assembler: “Compile this for 16-bit mode.”  
Why? Because BIOS loads our bootloader in **16-bit Real Mode**, not 32-bit.

---

### 🧠 `[ORG 0x7C00]`  
This tells the assembler:  
> "Pretend this code starts at address 0x7C00 in memory."  
That’s where BIOS loads our bootloader by default.

---

### 🏁 `start:`  
Just a label marking the entry point. BIOS starts executing here.

---

### 🛑 `cli` – **Clear Interrupts**  
Temporarily **disables hardware interrupts** (like keyboard input or timers).  
**Why?**  
- We're switching the CPU mode — even a small interrupt during this process could crash the system.

---

### 🧭 `lgdt [gdt_descriptor]` – **Load Global Descriptor Table**  
Loads the **memory segmentation table** we defined in `gdt.asm`.  
- Required before entering Protected Mode.  
- Tells the CPU: "Here’s how I’ll be organizing memory from now on."

---

### 🧠 `mov eax, cr0`  
Reads the **control register CR0** into the general-purpose register `EAX`.

---

### ⚙️ `or eax, 1`  
Turns on the **Protected Mode Enable (PE)** bit by setting bit 0 of `CR0`.

---

### 🧠 `mov cr0, eax`  
Writes the modified value back into `CR0`, officially **activating Protected Mode**.  
> ⚠️ At this point, our CPU is now in Protected Mode — but still executing old 16-bit code until...

---

### 🧠 `jmp 0x08:init_pm` – **Far Jump to 32-bit Code**
- `0x08` is the selector for the **code segment** in our GDT.
- `init_pm` is the label for our 32-bit code.

This **far jump** does two things:
1. Changes the instruction pointer (EIP)
2. Sets CS (Code Segment register) to 32-bit segment

Boom! We’re now in **32-bit mode**. 🚀

---

### 💻 `[BITS 32]`  
Tells the assembler: “From here on, this is 32-bit code.”

---

### 📍 `init_pm:`  
Label for where our 32-bit kernel begins executing.

---

### 🧠 `mov ax, 0x10`  
0x10 is the selector for our **data segment** in the GDT.

---

### 📦 `mov ds/es/fs/gs/ss, ax`  
Sets **all the segment registers** to point to the data segment.  
This is mandatory — or our CPU might crash when accessing memory.

---

### 🧠 `mov esp, 0x90000` – **Set Up Stack**
- ESP is the **stack pointer**.
- We're saying: "Let’s use the memory starting from 0x90000 for function calls, local variables, etc."

Why 0x90000?  
- It’s above our bootloader & kernel, so we avoid overwriting them.

---

### 🧠 `call kernel_main` – **Call the Kernel**
This is the C++ function that our kernel will define in `kernel.cpp`.  
For now, it's just a placeholder — but soon it’ll do real work!

---

### 🔁 `hang:`  
An infinite loop, to prevent the CPU from running into garbage instructions after kernel_main returns.

```nasm
hang:
    jmp hang
```

---

### 🧠 `kernel_main:`  
Dummy label that just `ret`s. Later, we’ll replace this with the actual kernel function from our C++ file.

---

### 📦 `%include "gdt.asm"`  
Includes the **GDT definition** we wrote earlier. This is critical — no GDT, no Protected Mode.

---

### 🧱 `times 510 - ($ - $$) db 0`  
Pads the rest of the bootloader to **exactly 510 bytes**.

> BIOS requires the bootloader to be **512 bytes** total.

---

### 🔚 `dw 0xAA55`  
This is the **boot signature**.

> BIOS checks for this before booting our code.  
If it’s not here, our bootloader won’t run.

---

## ✅ Summary of What We’ve Done

We:
- Disabled interrupts
- Loaded a custom memory model (GDT)
- Flipped the CPU into 32-bit mode
- Set up a proper stack
- Jumped into the kernel

This is the **critical transition** from BIOS land into our real operating system.
