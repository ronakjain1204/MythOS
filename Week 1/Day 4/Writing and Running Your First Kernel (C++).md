# **🧭Day 4: Writing and Running our First Kernel (C++)**

Hey everyone! Ronak here. Today is super exciting because we're going to write our first **real kernel in C++**! 🎉 

After spending the past three days setting up our environment and creating a bootloader, we're finally ready to build something that feels like a proper operating system. Let's dive in and create a kernel that displays a message on screen!

---

## **✅ Goals for Day 4**
- Create our first real kernel in C++.
- Display the message: "MythOS is alive!" on the screen.
- Compile, link, and boot it in QEMU.
- Learn file creation & editing in Linux.

---

## **🧰 Step-by-Step Breakdown**

### **🗂️ Step 1: Open Terminal and Go to Your Project Folder**
First, let's navigate to our MythOS project folder:

```bash
cd ~/Desktop/MythOS
```

This takes us to the MythOS project folder we created earlier. Make sure you're in the right directory before proceeding!

---

### **📄 Step 2: Create kernel.cpp File**
Now, let's create our first kernel file in C++:

```bash
touch kernel.cpp
```

This creates a blank file named `kernel.cpp`. To edit it, I'll use the nano text editor:

```bash
nano kernel.cpp
```

Now, let's paste the following code:

```cpp
extern "C" void kernel_main() {
    char* video_memory = (char*) 0xB8000;

    const char* message = "MythOS is alive!";
    int i = 0;

    while (message[i] != '\0') {
        video_memory[i * 2] = message[i];     // Character
        video_memory[i * 2 + 1] = 0x0F;        // White on black
        i++;
    }

    while (1); // Infinite loop to hang
}
```

After pasting:
- Press **CTRL + O** → **Enter** to save
- Press **CTRL + X** to exit nano

**What this code does:**
- `extern "C"` ensures C++ name mangling doesn't affect our function name.
- `0xB8000` is the memory address for VGA text mode display.
- We write each character and its color attribute (white on black) to video memory.
- The infinite loop at the end prevents the CPU from executing random memory.

---

### **🧾 Step 3: Create linker.ld File**
Next, we need a linker script to tell the compiler where to place our code in memory:

```bash
touch linker.ld
nano linker.ld
```

Paste this linker script:

```ld
ENTRY(kernel_main)

SECTIONS {
    . = 0x1000;

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }
}
```

Then save and exit (CTRL + O, Enter, CTRL + X).

**What this script does:**
- Sets `kernel_main` as the entry point.
- Loads our code starting at memory address `0x1000`.
- Organizes our code into standard sections (text, data, bss).

---

### **🧰 Step 4: Update the Makefile**
If you don't have a Makefile yet, let's create one:

```bash
touch Makefile
nano Makefile
```

Paste this Makefile:

```make
all: build/bootloader.bin

build/bootloader.bin: bootloader.asm kernel.cpp linker.ld
	mkdir -p build
	nasm -f bin bootloader.asm -o build/bootloader_stage1.bin
	i686-elf-g++ -ffreestanding -m32 -c kernel.cpp -o build/kernel.o
	i686-elf-ld -m elf_i386 -T linker.ld -o build/kernel.bin build/kernel.o
	cat build/bootloader_stage1.bin build/kernel.bin > build/bootloader.bin

run:
	qemu-system-i386 -drive format=raw,file=build/bootloader.bin

clean:
	rm -rf build
```

Then save and exit.

**What this Makefile does:**
1. Creates a build directory if it doesn't exist.
2. Compiles our bootloader with NASM.
3. Compiles our kernel with the cross-compiler.
4. Links the kernel using our linker script.
5. Combines the bootloader and kernel into one binary.
6. Provides a `run` target to test in QEMU.

---

### **🔧 Step 5: Build and Run Your OS**
Before we build, make sure you have all the required tools:
- nasm
- i686-elf-g++ and i686-elf-ld (cross compiler)
- qemu-system-i386

If you need to install them:

```bash
sudo apt install nasm qemu
```

For the cross-compiler setup, it's a bit more involved. If you haven't set it up yet, let me know in the comments, and I'll provide detailed instructions!

Now, let's build and run our OS:

```bash
make
make run
```

If all goes well, you'll see:

```
MythOS is alive!
```

displayed in a black QEMU window. 🎉

---

## **🔍 How It All Works**

| Component | Explanation |
|-----------|-------------|
| kernel.cpp | Contains your OS logic (displays text) |
| linker.ld | Tells the system where to load code |
| bootloader.asm | Kicks off the boot and jumps to C++ code |
| Makefile | Automates building and running |
| video_memory | Direct memory writing to the screen (VGA) |

The magic happens when our bootloader (which we wrote in Assembly) hands control over to our C++ kernel. The kernel then writes directly to video memory at address `0xB8000`, which is the standard location for VGA text mode display.

---

## **🧩 Common Issues**

| Problem | Fix |
|---------|-----|
| "booting from hard disk..." only | Check that build/bootloader.bin is generated |
| No text on screen | Verify correct VGA memory usage at 0xB8000 |
| Compile errors | Confirm you're using i686-elf-g++, not system g++ |

If you encounter any other issues, drop a comment below, and I'll help troubleshoot!

---

## **🎯 Summary of Day 4**
✅ Learned how to create & edit files in Linux  
✅ Wrote our first C++ kernel  
✅ Displayed real output from our OS  
✅ Ran our custom kernel with QEMU  

This is a huge milestone! We've now created a working (albeit simple) operating system that boots up and displays a message. In the coming days, we'll add more features like keyboard input, a command-line interface, and more.


---

**Next up:** Day 5, where we'll enhance our kernel with more features and start building a proper command-line interface!
