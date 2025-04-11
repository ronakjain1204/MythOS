# 🧭 **MythOS — Day 5: Adding Keyboard Input (Interrupt Handling)**

Hey everyone! Ronak here with Day 5 of our MythOS journey. Today is super exciting because we're going to make our OS **interactive** by handling keyboard input! 🎉

This is a big step forward—we're moving from a static display to actually responding to user input. Let's dive in and learn how to capture those keystrokes!

---

## 🧠 **What We'll Learn Today**
- What **interrupts** are (specifically hardware interrupts).
- How the **keyboard interrupt** works (IRQ1).
- How to write and register an **Interrupt Service Routine (ISR)**.
- How to set up and use the **Interrupt Descriptor Table (IDT)**.
- How to modify our kernel to **display keypresses** on screen.

---

## ⚙️ **Step-by-Step Implementation**

### 🧩 **Step 1: Understanding Interrupts (Beginner Explanation)**

**Interrupts** are signals from hardware (or software) that tell the CPU:  
> "Stop what you're doing. I need your attention RIGHT NOW!"

Think of interrupts like this:
- You're working on something important (the CPU executing code).
- Your phone rings (an interrupt occurs).
- You pause your work to answer the call (CPU runs an interrupt handler).
- After the call, you go back to what you were doing (CPU resumes normal execution).

For example:
- Pressing a **keyboard key** sends IRQ1 → triggers an **interrupt**.
- The CPU pauses what it was doing and runs a special function to handle it → an **Interrupt Service Routine (ISR)**.

To handle keyboard input, we need:
1. **IDT**: A table mapping interrupt numbers to their handler functions.
2. **ISR for IRQ1**: To handle keypresses.
3. **Port reading**: To get which key was pressed.

---

### 🗂️ **Step 2: File Structure Update**

Inside your `MythOS/` folder, let's make a new folder called `include` for header files:

```bash
mkdir include
touch include/idt.h include/isr.h
touch idt.cpp isr.cpp
```

This organization helps keep our code clean and modular as our OS grows.

---

### 📄 **Step 3: IDT Setup (Interrupt Descriptor Table)**

First, let's create the header file for our Interrupt Descriptor Table:

#### 🧱 `include/idt.h`

```cpp
#pragma once
#include 

struct IDTEntry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct IDTPointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void init_idt();
```

This header defines the structure of an IDT entry and a pointer to the IDT. The `__attribute__((packed))` ensures the compiler doesn't add padding between fields.

Now, let's implement the IDT initialization:

#### 🧠 `idt.cpp`

```cpp
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
```

Here, we:
- Declare an array of 256 IDT entries (one for each possible interrupt).
- Create a function to set up individual entries.
- Initialize the IDT pointer and load it with an assembly function we'll write later.

---

### 💥 **Step 4: Write `isr.h` and `isr.cpp` (Interrupt Handlers)**

Next, let's set up our Interrupt Service Routines:

#### 📄 `include/isr.h`

```cpp
#pragma once
void isr_install();
```

#### ⚙️ `isr.cpp`

```cpp
#include "include/idt.h"
#include "include/isr.h"

extern "C" void keyboard_handler();

void isr_install() {
    set_idt_entry(0x21, (uint32_t)keyboard_handler, 0x08, 0x8E);
}
```

This code:
- Declares a function `keyboard_handler()` that we'll implement in assembly.
- Sets up entry 0x21 (IRQ1 + 0x20) in the IDT to point to our keyboard handler.
- Uses selector 0x08 (our code segment) and flags 0x8E (present, ring 0, interrupt gate).

---

### 🔡 **Step 5: Keyboard Handler (Assembly)**

Now, let's write the assembly code that will handle keyboard interrupts:

```bash
touch isr.asm
```

Paste this code:

```nasm
[bits 32]

global keyboard_handler
extern keyboard_callback

keyboard_handler:
    pusha                   ; Push all registers

    call keyboard_callback  ; Call our C++ handler

    ; Send EOI (End of Interrupt) to PIC
    mov al, 0x20
    out 0x20, al

    popa                    ; Restore all registers
    iretd                   ; Return from interrupt
```

This assembly code:
- Saves all registers with `pusha`.
- Calls our C++ function `keyboard_callback`.
- Sends an EOI (End of Interrupt) signal to the PIC (Programmable Interrupt Controller).
- Restores registers and returns from the interrupt.

---

### 🧠 **Step 6: Read Key from Keyboard (C++)**

Now, let's update our `kernel.cpp` to handle keyboard input:

```cpp
#include "include/idt.h"
#include "include/isr.h"

extern "C" void kernel_main() {
    char* video_memory = (char*) 0xB8000;

    const char* message = "MythOS is alive!";
    int i = 0;

    while (message[i] != '\0') {
        video_memory[i * 2] = message[i];     // Character
        video_memory[i * 2 + 1] = 0x0F;        // White on black
        i++;
    }

    init_idt();
    isr_install();

    // Enable interrupts
    asm volatile("sti");

    // Initialize PIC
    // Remap IRQs 0-7 to interrupts 0x20-0x27
    // Remap IRQs 8-15 to interrupts 0x28-0x2F
    asm volatile("outb %%al, %%dx" : : "a"(0x11), "d"(0x20));
    asm volatile("outb %%al, %%dx" : : "a"(0x11), "d"(0xA0));
    asm volatile("outb %%al, %%dx" : : "a"(0x20), "d"(0x21));
    asm volatile("outb %%al, %%dx" : : "a"(0x28), "d"(0xA1));
    asm volatile("outb %%al, %%dx" : : "a"(0x04), "d"(0x21));
    asm volatile("outb %%al, %%dx" : : "a"(0x02), "d"(0xA1));
    asm volatile("outb %%al, %%dx" : : "a"(0x01), "d"(0x21));
    asm volatile("outb %%al, %%dx" : : "a"(0x01), "d"(0xA1));
    asm volatile("outb %%al, %%dx" : : "a"(0x0), "d"(0x21));
    asm volatile("outb %%al, %%dx" : : "a"(0x0), "d"(0xA1));

    while (1); // Infinite loop to hang
}

extern "C" void keyboard_callback() {
    unsigned char scancode;
    asm volatile ("inb %1, %0" : "=a"(scancode) : "Nd"(0x60));

    char* video_memory = (char*) 0xB8000;
    video_memory[160] = 'A';  // Just write 'A' at next line
    video_memory[161] = 0x0F;
}
```

We've added:
- Initialization of the IDT and ISRs.
- Enabling interrupts with `sti`.
- PIC initialization to remap IRQs.
- A keyboard callback that reads the scancode from port 0x60 and displays 'A' on screen.

For now, we're just displaying 'A' when any key is pressed. We'll decode real keys in the next session.

---

### 🔧 **Step 7: Modify `Makefile` to Include All Files**

Update your `Makefile` to compile and link all our new files:

```make
all: build/bootloader.bin

build/bootloader.bin: bootloader.asm kernel.cpp linker.ld idt.cpp isr.cpp isr.asm load_idt.asm
	mkdir -p build
	nasm -f bin bootloader.asm -o build/bootloader_stage1.bin
	i686-elf-g++ -ffreestanding -m32 -c kernel.cpp -o build/kernel.o
	i686-elf-g++ -ffreestanding -m32 -c idt.cpp -o build/idt.o
	i686-elf-g++ -ffreestanding -m32 -c isr.cpp -o build/isr.o
	nasm -f elf isr.asm -o build/isr_asm.o
	nasm -f elf load_idt.asm -o build/load_idt.o
	i686-elf-ld -m elf_i386 -T linker.ld -o build/kernel.bin build/kernel.o build/idt.o build/isr.o build/isr_asm.o build/load_idt.o
	cat build/bootloader_stage1.bin build/kernel.bin > build/bootloader.bin

run:
	qemu-system-i386 -drive format=raw,file=build/bootloader.bin

clean:
	rm -rf build
```

---

### 🛠️ **Step 8: Load IDT in Assembly**

Create `load_idt.asm`:

```nasm
[bits 32]

global load_idt

load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret
```

This simple function:
- Takes a pointer to the IDT descriptor from the stack.
- Loads it using the `lidt` instruction.
- Returns to the caller.

---

### 🧪 **Step 9: Build and Run**

Now, let's build and run our OS:

```bash
make clean
make
make run
```

When QEMU starts, you should see:
1. "MythOS is alive!" displayed at the top.
2. When you press any key, an 'A' should appear on the next line!

This means our interrupt handling is working correctly! 🎉

---

## 🔍 **Day 5 Summary**

| What We Did | What It Means |
|-------------|---------------|
| 🧠 Learned about interrupts | CPU reacting to events like keypress |
| 🛠️ Built an IDT | So CPU knows what to do when interrupts occur |
| 🎹 Wrote keyboard handler | Captured real hardware input |
| 💬 Displayed output | We're reading from real hardware input now! |

---


---

I hope you're as excited as I am about our progress! We've moved from a static display to actually handling user input—a huge step toward a fully interactive operating system.
