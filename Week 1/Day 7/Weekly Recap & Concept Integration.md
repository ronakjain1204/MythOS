# 📘 MythOS — Day 7: Weekly Recap & Concept Integration

Hey everyone! Ronak here. Welcome to Day 7 of our MythOS journey! 🎉

Today marks the end of our first week building MythOS from scratch. We've covered a tremendous amount of ground—from setting up our development environment to writing a bootloader, switching to Protected Mode, and even beginning to handle interrupts!

This day is dedicated to **solidifying our foundation**, **making sense of all the moving parts**, and getting mentally prepared for Week 2. Let's take a step back and review everything we've accomplished.

---

## 🎯 Day 7 Objective:

> 🧠 **Review + Summarize + Visualize + Document our understanding of Week 1**  
> 🎯 **Get clarity on core boot and kernel concepts before building real functionality**

---

## 📚 What We've Built by the End of Week 1

Let's take a moment to appreciate everything we've created in just one week:

| Component         | Description                                                                 |
|-------------------|-----------------------------------------------------------------------------|
| `bootloader.asm`  | Starts execution from BIOS, sets up Protected Mode, and jumps to the kernel |
| `gdt.asm`         | Global Descriptor Table setup for protected memory segments                 |
| `kernel.cpp`      | Our kernel's entry point (`kernel_main`)                                   |
| `idt.cpp` & `isr.asm` | Setup for interrupt handling with IDT and ISRs                           |
| `load_idt.asm`    | Loads the IDT into the CPU using the `lidt` instruction                     |
| `linker.ld`       | Controls where in memory the kernel is loaded                               |
| `Makefile`        | Automates the entire build and run process                                  |
| `build/` folder   | Holds our compiled files, ISO, and bootloader binary                       |

We now have an **OS skeleton** that can:
- Boot from BIOS
- Enter Protected Mode
- Jump to our kernel
- Begin setting up interrupt handling

👏 That's a *HUGE* accomplishment for one week! Let's take some time to make sure we fully understand what we've built.

---

## 🧩 Day 7: Full Recap Plan

I've split Day 7 into 5 parts to help us thoroughly review and solidify our understanding:

---

### 🧹 Part 1: Clean & Rebuild

First, let's ensure everything is working correctly by cleaning and rebuilding our project:

```bash
make clean
make
make run
```

When I ran this on my system, I confirmed that our OS boots in QEMU and enters Protected Mode successfully. If you're seeing any issues, now is a great time to troubleshoot them!

---

### 📌 Part 2: File-by-File Recap (Cheat Sheet Style)

Let's go through each file we've created and understand its purpose:

#### 🗂 `bootloader.asm`
- BIOS loads the first 512 bytes here at memory address `0x7C00`.
- Sets up the GDT (Global Descriptor Table).
- Switches the CPU from Real Mode (16-bit) to Protected Mode (32-bit).
- Jumps to the 32-bit kernel.
- Must end with the boot signature `0xAA55`.

#### 🗂 `gdt.asm`
- Defines memory segments (code and data).
- Required for the CPU to safely access memory in Protected Mode.
- Contains three entries: null descriptor, code segment, and data segment.
- Each entry is 8 bytes and defines base address, limit, access rights, etc.

#### 🗂 `kernel.cpp`
- Contains `extern "C" void kernel_main()`, which is our kernel's entry point.
- Initializes the IDT (Interrupt Descriptor Table) and ISRs (Interrupt Service Routines).
- Currently displays "MythOS is alive!" on screen.
- Will eventually contain all our OS logic.

#### 🗂 `idt.cpp` + `include/idt.h`
- IDT = table for telling the CPU where to jump when interrupts occur.
- Contains functions to set up each IDT entry.
- Maps interrupt numbers to handler functions.
- Essential for handling hardware events like keyboard input.

#### 🗂 `isr.asm` + `include/isr.h`
- These are the actual low-level interrupt handler functions.
- Connected to entries in the IDT.
- Save CPU state, call C++ handlers, restore state, and return.
- Currently set up to handle keyboard interrupts.

#### 🗂 `load_idt.asm`
- Uses the `lidt` instruction to tell the CPU "here's your interrupt table."
- A small but crucial bridge between C++ and Assembly.

#### 🗂 `linker.ld`
- Ensures the kernel is loaded to the correct memory address (e.g., `0x1000`).
- Defines the layout of our kernel in memory.
- Kernel is a flat binary, not an ELF or EXE file.

#### 🗂 `Makefile`
- Compiles and links everything.
- Builds the bootloader, kernel, and ISO.
- Runs it with QEMU.
- Automates the entire build process.

---

### 📊 Part 3: Visual Map of the Boot Process

Here's a visual representation of our boot process:

```plaintext
[ BIOS (ROM) ]
        ↓
[ bootloader.asm @ 0x7C00 ]
        ↓
[GDT setup]
        ↓
[Protected Mode Entered]
        ↓
[JMP to kernel_main @ 0x1000]
        ↓
[kernel.cpp]
  ├─> IDT Setup
  ├─> ISRs Setup
  └─> Display "MythOS is alive!"
```

This flow shows how control transfers from the BIOS to our bootloader, then to our kernel. Understanding this sequence is crucial for debugging and extending our OS.

---

### 🧠 Part 4: Conceptual Review Questions

To check my understanding (and hopefully help yours too!), I've answered these key questions:

1. **What happens when the computer powers on?**
   - The CPU starts executing code from a fixed memory address.
   - The BIOS performs POST (Power-On Self Test) to check hardware.
   - The BIOS looks for a bootable device and loads the first 512 bytes (our bootloader) into memory at address 0x7C00.
   - Control is transferred to our bootloader.

2. **Why must the bootloader be 512 bytes with `0xAA55` signature?**
   - 512 bytes is the size of a disk sector, the smallest unit the BIOS can read.
   - The `0xAA55` signature (at bytes 510-511) tells the BIOS this sector is bootable.
   - Without this signature, the BIOS won't recognize our code as a bootloader.

3. **What does Protected Mode offer compared to Real Mode?**
   - Access to the full 4GB of memory (vs. 1MB in Real Mode).
   - Memory protection features to prevent programs from interfering with each other.
   - 32-bit registers and operations (vs. 16-bit in Real Mode).
   - Support for modern features like paging and multitasking.

4. **What is a GDT entry composed of? What do the flags mean?**
   - Base address: Where the segment starts in memory.
   - Limit: Size of the segment.
   - Access byte: Defines privileges, type (code/data), and read/write permissions.
   - Flags: Control granularity, size (16/32-bit), and other properties.

5. **What is the purpose of an IDT?**
   - The IDT maps interrupt numbers to handler functions.
   - When an interrupt occurs, the CPU looks up the corresponding entry in the IDT.
   - The IDT tells the CPU where to find the code that handles each type of interrupt.

6. **How do ISRs relate to the IDT?**
   - ISRs (Interrupt Service Routines) are the functions that handle interrupts.
   - The IDT contains pointers to these ISRs.
   - When an interrupt occurs, the CPU uses the IDT to find and execute the appropriate ISR.

7. **Why is `extern "C"` used in kernel_main?**
   - C++ compilers "mangle" function names to support overloading.
   - `extern "C"` prevents this name mangling.
   - This ensures our bootloader (written in Assembly) can find and call `kernel_main` correctly.

8. **Why is a linker script necessary?**
   - The linker script controls where in memory our kernel is placed.
   - It defines the layout of different sections (.text, .data, .bss).
   - Without it, our kernel might not be loaded at the address our bootloader expects.

---

### 📝 Part 5: Documentation & Summary

I've created a `NOTES.md` file in our project to document key concepts and components. Here's a summary of what I included:

#### Key Terms:
- **BIOS**: Basic Input/Output System - firmware that initializes hardware and starts the boot process.
- **GDT**: Global Descriptor Table - defines memory segments for Protected Mode.
- **IDT**: Interrupt Descriptor Table - maps interrupt numbers to handler functions.
- **ISR**: Interrupt Service Routine - code that handles specific interrupts.
- **Protected Mode**: 32-bit CPU mode with memory protection and extended features.
- **Bootloader**: Code that initializes the system and loads the kernel.
- **Kernel**: The core of the operating system that manages resources and provides services.

#### What happens in `bootloader.asm`:
1. BIOS loads it at address 0x7C00.
2. It sets up the GDT for Protected Mode.
3. It enables Protected Mode by setting the PE bit in CR0.
4. It performs a far jump to flush the pipeline and load the new code segment.
5. It initializes segment registers for Protected Mode.
6. It jumps to `kernel_main`.

#### What `kernel_main` currently does:
1. Displays "MythOS is alive!" on screen by writing to video memory.
2. Sets up the IDT for interrupt handling.
3. Installs ISRs for keyboard input.
4. Enables interrupts and initializes the PIC.
5. Enters an infinite loop to keep the system running.

#### Folder Structure:
```
MythOS/
├── Makefile
├── bootloader.asm
├── kernel.cpp
├── linker.ld
├── idt.cpp
├── load_idt.asm
├── isr.asm
├── include/
│   ├── idt.h
│   └── isr.h
├── build/
│   ├── bootloader.bin
│   ├── kernel.bin
│   └── os-image.iso
```

---

## 🚀 What's Next in Week 2?

Now that we have a solid foundation, Week 2 will focus on making our OS more interactive and functional:

- Printing colored text on the screen
- Handling keyboard input
- Basic memory management (segmenting and paging preview)
- Diving deeper into interrupt handling

We'll start transforming MythOS from a skeleton into a real, interactive operating system! 🎉

---

## ✅ Day 7 Summary Checklist

| Task | Status |
|------|--------|
| Clean build & run the OS | ✅ |
| Review all files | ✅ |
| Understand boot flow | ✅ |
| Answer concept questions | ✅ |
| Create notes / visuals | ✅ |
| Ready for Week 2 | 🎉 |

---

## 🎉 Wrapping Up Week 1

I'm incredibly proud of what we've accomplished in just one week. We've gone from nothing to a functioning OS skeleton that boots, enters Protected Mode, and is ready to handle interrupts!

Building an operating system from scratch is no small feat, and we're well on our way. The foundation we've laid this week will support all the exciting features we'll add in the weeks to come.

What part of Week 1 did you find most challenging? What are you most looking forward to in Week 2? Let me know in the comments below!

Thank you for joining me on this journey. Let's keep building MythOS together! 💻✨

See you in Week 2! 🚀
