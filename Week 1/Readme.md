# **Week 1: Bootstrapping MythOS**

Welcome to **Week 1** of the **MythOS Development Series!** 🎉  
This week is all about laying the foundation for our operating system. By the end of this week, we will have a working **bootloader** and a **basic kernel**, marking the first steps toward building MythOS from scratch.

---

## **📚 What We Accomplished This Week**

### **1. Set Up the Development Environment**
- Installed essential tools like:
  - **NASM** (Assembler)
  - **QEMU** (Virtual Machine)
  - **GCC (Cross Compiler)**
  - **Make** (Build Automation)
- These tools were used to build and test the OS.

---

### **2. Learned How Computers Boot**
- Explored the boot process, including:
  - **BIOS Initialization**: Hardware checks and boot device selection.
  - **MBR Execution**: Loading the first 512 bytes from the bootable device.
  - **Bootloader Loading**: Preparing the system for kernel execution.
  - **Kernel Execution**: Managing hardware, memory, and processes.

---

### **3. Wrote a Custom Bootloader in Assembly**
- Created a simple bootloader that prints `"MythOS"` on the screen.
- Learned how to:
  - Fill the boot sector to exactly 512 bytes.
  - Include a valid boot signature (`0xAA55`).

---

### **4. Transitioned from Real Mode to Protected Mode**
- Modified the bootloader to switch the CPU from:
  - **Real Mode (16-bit)** → Limited memory and outdated features.
  - **Protected Mode (32-bit)** → Full memory access and modern features.
- Set up a **Global Descriptor Table (GDT)** for memory segmentation.

---

### **5. Built a Basic Kernel in C++**
- Wrote a simple kernel that prints `"Welcome to MythOS!"` using VGA text mode.
- Compiled and linked the kernel using GCC.
- Tested it in QEMU to ensure proper functionality.

---

## **🛠 Tools Used**
- **NASM**: To write and assemble low-level Assembly code for the bootloader.
- **QEMU**: To test the bootloader and kernel in a virtual machine.
- **GCC (Cross Compiler)**: To compile the kernel in C++.
- **Make**: To automate and simplify the build process.

---

## **✅ Key Outputs**
By the end of Week 1, we achieved:
1. A custom bootloader that prints `"MythOS"` on screen.  
2. A transition from Real Mode to Protected Mode using GDT.  
3. A basic kernel written in C++ that displays `"Welcome to MythOS!"` on screen.

---

## **🚀 Next Steps**
In Week 2, we’ll expand on this foundation by:
1. Handling keyboard input through interrupts.  
2. Building a simple command-line interface (CLI).  
3. Adding basic commands like `clear`, `echo`, and `help`.  

Stay tuned as we continue building MythOS step by step! 😊
