# **📌 Week 1 - Day 1: Setting Up the OS Development Environment**

Today, we will **set up everything needed** to start building our operating system. Since we are complete beginners, we'll **explain each concept in depth** to ensure we understand why we're doing each step.

---

## **🛠️ What We'll Achieve Today**
- Understand **how a computer boots** step-by-step.
- Install all necessary **tools** for OS development.
- Set up a **project structure** where we'll write our OS.

By the end of today, we'll have a fully configured system and be ready to start coding the bootloader.

---

## **🖥️ Step 1: How a Computer Boots**

Before we start coding, let's understand what happens when a computer turns on.

### **🔹 Step-by-Step Booting Process**
1. **CPU Starts Execution at a Fixed Memory Location**
   - The CPU starts execution from a pre-defined memory address.
   - On x86 systems, this is **0xFFFF0**, near the end of system memory.

2. **BIOS (or UEFI) Runs**
   - The BIOS performs a **Power-On Self Test (POST)** to check hardware (CPU, RAM, etc.).
   - It looks for a **bootable device** (HDD, SSD, USB).

3. **BIOS Loads the Bootloader**
   - BIOS searches for a bootloader in the first sector (512 bytes) of a bootable device.
   - This special area is called the **Master Boot Record (MBR)**.

4. **Bootloader Loads the Kernel**
   - The bootloader finds and loads the OS kernel into memory.
   - The kernel takes control and starts the operating system.

✅ **Summary:**  
The CPU starts → BIOS runs → BIOS loads bootloader → Bootloader loads Kernel → OS starts.

---

## **🛠️ Step 2: Installing Required Tools**

To develop an OS, we need some essential tools. Let's install them and understand their purpose.

### **🔹 List of Required Tools**
| Tool                 | Purpose                                                                 |
|----------------------|-------------------------------------------------------------------------|
| **NASM**             | Assembler to compile bootloader code (written in Assembly).            |
| **GCC (Cross Compiler)** | Compiles C++ kernel code for bare-metal environments.                  |
| **QEMU**             | Emulator to test our OS without rebooting the PC.                      |
| **Make**             | Automates the build process.                                           |

---

### **🔹 Installation Commands**
#### 📌 On Linux (Ubuntu/Debian) or WSL:
```
sudo apt update
sudo apt install nasm gcc g++ make qemu qemu-system-x86
```

#### 📌 On macOS:
```
brew install nasm qemu
```

#### 📌 On Windows:
1. [Download NASM](https://www.nasm.us/)  
2. [Download MinGW-w64](https://www.mingw-w64.org/)  
3. [Download QEMU for Windows](https://qemu.weilnetz.de/)

---

### **🔹 Verification**
After installation, we'll verify that all tools are installed correctly by running:
```
nasm -v
gcc --version
qemu-system-x86_64 --version
```
If these commands return version numbers, our installation is successful!

---

## **📂 Step 3: Setting Up the Project Structure**

Let's create a clean folder structure for our project to keep everything organized.

### Create Project Folder:
```
mkdir MythOS
cd MythOS
```

### Inside `MythOS`, create these files:
```
MythOS/
├── bootloader.asm   # Bootloader in Assembly
├── kernel.cpp       # Kernel in C++
├── Makefile         # Automates the build process
├── build/           # Directory for compiled files
```

This structure will help us manage our code as we add more components to MythOS.

---

## **✅ Summary of Day 1**
✔️ Understood how a computer boots step-by-step.  
✔️ Installed all necessary tools for OS development.  
✔️ Set up a clean project folder structure.  

---

## **🚀 What's Next?**
📌 Tomorrow (**Day 2**) we will:  
1. Write our first full bootloader in NASM Assembly.  
2. Compile and test it using QEMU.  
3. Learn how to transition from Real Mode to Protected Mode.

🔥 We're ready to start writing real OS code!
