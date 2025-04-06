
# **📌 Week 1 - Day 3: Switching from Real Mode to Protected Mode**

Hey everyone! Ronak here with today's exciting milestone in our MythOS journey. 😊 Today, we're taking a huge leap forward by transitioning from a simple bootloader to setting up the foundation for a real, modern OS by switching from **Real Mode** to **Protected Mode**!

Let's dive in together and explore this crucial step in OS development!

---

## **🧭 What We'll Learn Today (Big Picture)**

Today, we'll walk through:

- What Real Mode and Protected Mode are (in human terms!)
- Why Protected Mode is essential for writing a modern OS
- What is the Global Descriptor Table (GDT) and why we need it
- How to modify our bootloader to:
  - Set up the GDT
  - Enable Protected Mode
  - Jump into 32-bit code

Don't worry if this sounds complicated—I'll break it down into simple concepts as we go along!

---

## **🧱 Step 1: Understanding CPU Modes**

Think of your CPU like a superhero with different costumes. The costume it wears determines its powers!

| Mode | Think of it as... | Description |
|------|-------------------|-------------|
| **Real Mode** | Your CPU in "old school 1980s costume" | Only 1MB memory, 16-bit registers, no safety features |
| **Protected Mode** | Your CPU in "modern superhero armor" | 4GB memory, 32-bit power, multitasking, security checks |

💡 When the computer turns on, it always starts in Real Mode, like waking up in pajamas. But we want it suited up (Protected Mode) before we start coding our OS.

### **❓ Why Can't We Write Our OS in Real Mode?**

In Real Mode:
- You only get 1MB of memory. 😱
- You can't use 32-bit registers (needed for C++).
- You have no memory protection—one error can destroy your whole system!

In Protected Mode:
- Access to full 4GB of memory
- Full 32-bit instructions
- Real safety features like memory segmentation and multitasking

So basically:
- 🛑 Real Mode = Fine for bootloaders
- ✅ Protected Mode = Required for kernels

---

## **🧪 Step 2: What Do We Need to Switch Modes?**

To switch to Protected Mode, we must:

1. Disable interrupts (we don't want the CPU to freak out mid-switch)
2. Define a memory layout using a table called the Global Descriptor Table (GDT)
3. Set the Protected Mode Enable (PE) bit in a special CPU register (CR0)
4. Perform a "far jump" (a special jump that reloads the code segment)

---

## **🧠 Step 3: What Is the GDT (Global Descriptor Table)?**

Think of memory like a huge building, and each room (segment) needs a keycard.

The GDT is the security system that:
- Defines where each room (memory segment) starts/ends
- Says what kind of room it is (Code/Data)
- Controls access (Read/Write/Execute)

We'll define 3 entries:
1. Null (a blank key)
2. Code Segment (0x08)
3. Data Segment (0x10)

---

## **🛠 Step 4: Update Folder Structure**

Let's organize our project properly. Inside our MythOS/ folder, I've added:

```bash
cd MythOS
touch gdt.asm gdt.h
```

Our new structure:

```
MythOS/
├── bootloader.asm   # Bootloader code (assembly)
├── gdt.asm          # GDT table and descriptor
├── gdt.h            # Later for C++ side (kernel)
├── kernel.cpp       # Kernel in C++
├── Makefile         # Build automation
├── build/           # Output binary goes here
```

---

## **✍️ Step 5: Write the GDT (gdt.asm)**

First, I created the GDT in a separate file for better organization. Here's my implementation in `gdt.asm`:

```nasm
gdt_start:
    dq 0x0000000000000000     ; Null descriptor (must be zero)
    dq 0x00CF9A000000FFFF     ; Code segment descriptor
    dq 0x00CF92000000FFFF     ; Data segment descriptor
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size of the GDT - 1
    dd gdt_start                ; Address of GDT
```

### **🔍 What's Going On Here?**
- `dq` = Define Quadword = 8 bytes
- First entry: Required null segment (unused)
- Second entry: Code segment descriptor
- Third entry: Data segment descriptor
- `gdt_descriptor`: A structure telling the CPU where the GDT lives

---

## **✍️ Step 6: Update bootloader.asm to Switch Modes**

Next, I modified our `bootloader.asm` to include the mode-switching code:

```nasm
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
```

---

## **🛠 Step 7: Build and Run**

After writing the code, I compiled and tested it with:

```bash
nasm -f bin bootloader.asm -o build/bootloader.bin
qemu-system-x86_64 -drive format=raw,file=build/bootloader.bin
```

### **✅ Output:**
You'll see a blank screen—but this is actually a success!

Why? Because:
- Our code didn't crash
- We successfully entered Protected Mode
- The kernel function (ret) returns, and the system halts in an infinite loop


| Concept | What We Did |
|---------|-------------|
| Real vs Protected Mode | Learned why Protected Mode is needed |
| GDT | Defined memory segments for 32-bit code |
| Bootloader | Modified it to enter Protected Mode |
| Testing | Successfully booted into 32-bit mode in QEMU |

---

## **📅 Next Up: Day 4 — Writing Your First C++ Kernel**

Tomorrow, we'll:
- Set up a cross-compiler
- Create kernel.cpp with a C++ main()-like function
- Link it with the bootloader
- Print a message: "Welcome to MythOS!"

I'm so excited to continue this journey with all of you! If you have any questions or run into issues with today's code, just open an issue in this repo, and I'll be happy to help. Let's keep building MythOS together! 🚀

---
