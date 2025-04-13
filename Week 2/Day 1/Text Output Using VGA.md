# **📘 MythOS — Week 2 Day 1: Text Output Using VGA**  

Hey everyone! Ronak here. Today marks the start of **Week 2** of our MythOS journey, and we're diving into one of the most exciting parts of OS development: **displaying text on the screen**! 🎉  

By the end of today, you'll have a kernel that prints **"Hello from MythOS!"** in glorious 32-bit Protected Mode. Let's get started! 😊  

---

## **🎯 Goal for Today**  
✅ Understand how VGA text mode works  
✅ Learn how video memory is structured (starting at `0xB8000`)  
✅ Write a simple function in C++ to display characters  
✅ Print "Hello from MythOS!" on boot  

---

## **🧠 Concept: VGA Text Mode**  
In **text mode**, your screen is divided into a grid of characters (typically **80 columns × 25 rows**). The VGA controller maps this grid to a special memory region:  

```
📍 Memory Address: 0xB8000
```

**Each character cell** uses **2 bytes**:  
| Byte 1          | Byte 2          |
|-----------------|-----------------|
| ASCII Character | Attribute (color info) |  

- **Attribute Byte**:  
  - **High nibble** (bits 4-7): Background color  
  - **Low nibble** (bits 0-3): Foreground color  

**Example**: `0x4F` = Red text (`0x4`) on Black background (`0x0`) with blinking (`0x8`).  

---

## **🛠 Step-by-Step Implementation**  

### **📂 Step 1: Project Structure**  
Ensure your project has these files:  
```
MythOS/
├── bootloader.asm    # Existing bootloader
├── kernel.cpp        # Today's focus
├── Makefile          # Updated build script
├── build/            # Output folder
```

---

### **✍️ Step 2: Update `kernel.cpp`**  
Replace your existing `kernel.cpp` with this code:  

```cpp
// kernel.cpp
#include 

// VGA Configuration
const uint16_t VGA_WIDTH = 80;
const uint16_t VGA_HEIGHT = 25;
uint16_t* video_memory = (uint16_t*) 0xB8000;  // VGA text buffer
uint16_t cursor_position = 0;                  // Tracks current screen position

// Combines ASCII character and color into a 16-bit VGA entry
uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color = VGA_WIDTH * VGA_HEIGHT) {
        // TODO: Implement screen scrolling later!
        return;
    }
    video_memory[cursor_position++] = vga_entry(c, color);
}

// Prints a null-terminated string
void print(const char* str, uint8_t color = 0x0F) {
    for (int i = 0; str[i] != '\0'; ++i) {
        putchar(str[i], color);
    }
}

// Kernel entry point
extern "C" void kernel_main() {
    print("Hello from MythOS!", 0x0F);  // White text on black background
    while (1);  // Infinite loop to keep the OS running
}
```

---

### **🔍 Code Explanation**  

#### **1. `vga_entry(char c, uint8_t color)`**  
- Combines a character and color into a 16-bit value the VGA understands.  
- Example: `vga_entry('A', 0x0F)` → `0x0F41` (ASCII 'A' = 0x41).  

#### **2. `putchar(char c, uint8_t color)`**  
- Writes a character to the current cursor position.  
- `cursor_position` tracks where the next character will go.  
- **Note**: We’ll implement scrolling in a future day!  

#### **3. `print(const char* str, uint8_t color)`**  
- Iterates through a string and prints each character.  
- Default color is white-on-black (`0x0F`).  

#### **4. `kernel_main()`**  
- Our OS’s starting point after the bootloader.  
- Calls `print()` to display our message.  

---

## **🎨 Color Table**  
Use these codes to customize text and background colors:  

| Code | Color       | Example Use Case          |
|------|-------------|---------------------------|
| 0x0  | Black       | Background                |
| 0x1  | Blue        | Error messages            |
| 0x2  | Green       | Success messages          |
| 0x3  | Cyan        | System info               |
| 0x4  | Red         | Warnings                  |
| 0x5  | Magenta     | Debug output              |
| 0x6  | Brown       | Deprecated functions      |
| 0x7  | Light Grey  | Comments                  |
| 0xF  | White       | Default text              |

**To set background color**: Shift left by 4 bits.  
Example: `0x10` = Blue background, `0x40` = Red background.  

---

## **⚙️ Step 3: Update the Makefile**  
Ensure your `Makefile` compiles the C++ code correctly:  

```make
# Makefile
all: os-image

os-image: build/bootloader.bin build/kernel.bin
	cat build/bootloader.bin build/kernel.bin > build/os-image.bin

build/bootloader.bin: bootloader.asm
	nasm -f bin bootloader.asm -o build/bootloader.bin

build/kernel.bin: kernel.cpp
	i686-elf-g++ -ffreestanding -c kernel.cpp -o build/kernel.o
	i686-elf-ld -Ttext 0x1000 -o build/kernel.elf build/kernel.o --oformat elf32-i386
	objcopy -O binary build/kernel.elf build/kernel.bin

clean:
	rm -f build/*.bin build/*.o build/*.elf
```

---

## **🧪 Step 4: Build & Test**  
Run these commands to build and test your OS:  

```bash
cd MythOS
make clean   # Start fresh
make         # Compile everything
qemu-system-i386 -fda build/os-image.bin
```

**Expected Output**:  
You should see **"Hello from MythOS!"** printed in white on a black screen!  

---

## **📝 Exercises (Optional but Recommended)**  
1. **Change the text color** to red (`0x04`) or green (`0x02`).  
2. **Print your name** instead of "MythOS".  
3. **Experiment with background colors**: Try `print("Test", 0x4F)` for red-on-white.  
4. **Write a multi-line message** (note: cursor won’t wrap yet!).  

---

## **🚀 What’s Next?**  
Tomorrow (**Day 2**), we’ll:  
- Add **screen-clearing functionality**  
- Implement **cursor movement** and **line wrapping**  
- Create helper functions for **colored text**  

---

Let me know if you run into any issues or have questions! Let’s make MythOS shine! 💻✨
