# **📘 MythOS — Week 2 Day 3: Scrolling and Screen Boundaries**  

Hey everyone! Ronak here. Today, we're tackling a crucial feature for any functional OS: **screen scrolling**! 🚀  

When our OS prints more lines than the screen can display, we want it to **scroll upwards** like a real terminal instead of clearing everything. Let’s implement this together! 😊  

---

## **🎯 Goal for Today**  
✅ Implement **screen scrolling** when text reaches the bottom  
✅ Preserve previous messages instead of abruptly clearing  
✅ Handle cursor boundaries and line wrapping  

---

## **🧠 Why Scrolling Matters**  
Without scrolling:  
- Text that overflows the screen disappears or forces a clear.  
- Users lose context of previous outputs.  
- The OS feels less professional and user-friendly.  

**Scrolling solves this!** It makes MythOS behave like a real terminal.  

---

## **🛠 Step-by-Step Implementation**  

### **📂 Step 1: Update `kernel.cpp`**  
We’ll add a `scroll_if_needed()` function and enhance `putchar()`.  

```cpp  
// kernel.cpp  
#include   

const uint16_t VGA_WIDTH = 80;  
const uint16_t VGA_HEIGHT = 25;  
uint16_t* video_memory = (uint16_t*) 0xB8000;  

uint16_t cursor_position = 0;  

// Combine character and color into a VGA entry  
uint16_t vga_entry(char c, uint8_t color) {  
    return (uint16_t)c | ((uint16_t)color > ", 0x0A); // Green  
        if (i < 10) {  
            putchar('0' + i);  
        } else {  
            putchar('1');  
            putchar('0' + (i - 10));  
        }  
        print("\n");  
    }  

    print("\n--- Scroll Test Complete ---\n", 0x0D); // Magenta  
    while (1);  
}  
```

---

### **🔍 Code Explanation**  

#### **1. `scroll_if_needed()`**  
- **Checks**: If the cursor is beyond the last row.  
- **Shifts Lines**: Moves each line up by one row (except the first).  
- **Clears Last Line**: Fills the bottom row with spaces to erase old content.  
- **Adjusts Cursor**: Moves the cursor back to the start of the last line.  

#### **2. Enhanced `putchar()`**  
- **Handles `\n` and `\r`**: Moves the cursor to the next line or start of the line.  
- **Calls Scroll Check**: After printing, checks if scrolling is needed.  

#### **3. `kernel_main()`**  
- Prints 30 lines to test scrolling.  
- Uses colors to differentiate line numbers and headers.  

---

## **🧪 Step 2: Build and Test**  
Run these commands to test scrolling:  

```bash  
cd MythOS  
make clean && make  
qemu-system-i386 -fda build/os-image.bin  
```

**Expected Output**:  
- A welcome message followed by 30 lines.  
- When the screen fills, new lines push older ones up.  
- The final message appears after scrolling.  

---

## **📝 Exercises (Optional but Recommended)**  
1. **Modify Scroll Speed**: Scroll by 2 lines at a time instead of 1.  
2. **Add Line Numbers**: Print line numbers on the left side.  
3. **Preserve Color**: Ensure scrolling retains the original text colors.  
4. **Implement Page Up/Down**: Advanced challenge for keyboard input later.  

---

## **🚀 What’s Next?**  
In **Day 4**, we’ll:  
- Implement **keyboard input handling**  
- Convert scan codes to readable characters  
- Allow users to type and see their input on the screen  

---
