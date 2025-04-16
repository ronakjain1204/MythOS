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
