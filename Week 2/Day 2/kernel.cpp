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
