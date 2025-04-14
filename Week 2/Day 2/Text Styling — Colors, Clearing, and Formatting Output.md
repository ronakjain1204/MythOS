# **📘 MythOS — Week 2 Day 2: Text Styling — Colors, Clearing, and Formatting Output**  

Hey everyone! Ronak here. Today, we’re enhancing MythOS with **text styling**, **screen clearing**, and **formatting**! By the end of today, you’ll have a colorful, organized display that feels like a real OS interface. Let’s dive in! 😊  

---

## **🎯 Goal for Today**  
✅ Add screen-clearing functionality  
✅ Implement newline (`\n`) and carriage return (`\r`) handling  
✅ Improve cursor positioning  
✅ Support custom text and background colors  

---

## **🧠 Concept: VGA Text Mode Advanced**  
Recall from Day 1:  
- **VGA Memory**: Starts at `0xB8000`  
- **Each Character**: 2 bytes (ASCII + color attribute)  
- **Screen Dimensions**: 80 columns × 25 rows  

Today, we’ll build on this by adding structure and style to our output.  

---

## **🛠 Step-by-Step Implementation**  

### **📂 Step 1: Update `kernel.cpp`**  
Replace your existing `kernel.cpp` with this enhanced version:  

```cpp  
// kernel.cpp  
#include   

const uint16_t VGA_WIDTH = 80;  
const uint16_t VGA_HEIGHT = 25;  
uint16_t* video_memory = (uint16_t*) 0xB8000;  
uint16_t cursor_position = 0;  

// Combine character and color into a VGA entry  
uint16_t vga_entry(char c, uint8_t color) {  
    return (uint16_t)c | ((uint16_t)color = VGA_WIDTH * VGA_HEIGHT) {  
        clear_screen(color);  
    }  
}  

// Print a string with color  
void print(const char* str, uint8_t color = 0x0F) {  
    for (int i = 0; str[i] != '\0'; ++i) {  
        putchar(str[i], color);  
    }  
}  

// Kernel entry point  
extern "C" void kernel_main() {  
    clear_screen(0x1F);  // Blue background, white text  
    print("Welcome to MythOS!\n", 0x1F);  
    print("Today's Agenda:\n", 0x0E);  // Yellow  
    print("- Clear screen\n", 0x0A);   // Green  
    print("- Colored text\n", 0x0C);   // Red  
    print("- Newlines\n\n", 0x0D);     // Magenta  
    print("Let's build! 🚀\n", 0x0B);  // Cyan  
    while (1);  
}  
```

---

### **🔍 Code Explanation**  

#### **1. `set_cursor_position(row, col)`**  
- Moves the cursor to a specific row and column.  
- Example: `set_cursor_position(2, 10)` moves to row 2, column 10.  

#### **2. `clear_screen(color)`**  
- Fills the entire screen with spaces and a background color.  
- Resets the cursor to the top-left corner.  

#### **3. Enhanced `putchar()`**  
- Handles `\n` (newline) by moving to the next row.  
- Handles `\r` (carriage return) by moving to the start of the current row.  
- Automatically clears the screen if the cursor exceeds the buffer (temporary fix).  

#### **4. `print()` with Color**  
- Prints a string with a specified color attribute.  
- Default color: white text on black background (`0x0F`).  

---

## **🎨 Color Attribute Reference**  
| Foreground | Background | Code (Hex) | Example             |  
|------------|------------|------------|---------------------|  
| White      | Black      | `0x0F`     | Default text        |  
| Blue       | Black      | `0x01`     | Error messages      |  
| Green      | Black      | `0x02`     | Success messages    |  
| Cyan       | Black      | `0x03`     | System info         |  
| Red        | Black      | `0x04`     | Warnings            |  
| Magenta    | Black      | `0x05`     | Debug output        |  
| Yellow     | Black      | `0x0E`     | Highlights          |  
| White      | Blue       | `0x1F`     | Header text         |  

**Formula**:  
```  
Attribute = (Background << 4) | Foreground  
```
Example: Blue background (`0x1`) + White text (`0xF`) = `0x1F`.  

---

## **⚙️ Step 2: Build and Test**  
Run these commands to test the new features:  

```bash  
cd MythOS  
make clean && make  
qemu-system-i386 -fda build/os-image.bin  
```

**Expected Output**:  
- A blue screen with a welcome message.  
- Colored lines demonstrating different text styles.  
- Properly aligned newlines and carriage returns.  

---

## **📝 Exercises (Optional but Recommended)**  
1. **Change the default background** to green (`0x20`) and text to black (`0x0`).  
2. **Print a multi-column layout** (e.g., two columns of text side-by-side).  
3. **Add a blinking cursor** by toggling the underscore character (`_`) at the cursor position.  
4. **Implement screen scrolling** when the cursor reaches the bottom (advanced).  

---
