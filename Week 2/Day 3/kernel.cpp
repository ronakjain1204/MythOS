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
