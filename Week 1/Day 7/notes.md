# MythOS Development Notes

## Key Terms and Concepts

### BIOS (Basic Input/Output System)
- Firmware that initializes hardware during boot process
- Performs Power-On Self Test (POST) to check hardware functionality
- Searches for bootable devices and loads the bootloader
- Executes from a fixed memory address when computer powers on

### Bootloader
- First 512 bytes loaded by BIOS at memory address 0x7C00
- Must end with boot signature 0xAA55 to be recognized as bootable
- Responsible for initializing basic hardware and loading the kernel
- In MythOS: Sets up GDT, switches to Protected Mode, jumps to kernel

### Real Mode vs. Protected Mode
- **Real Mode (16-bit)**:
  - Limited to 1MB of memory
  - No memory protection
  - 16-bit registers and operations
  - Legacy mode from early x86 processors
  
- **Protected Mode (32-bit)**:
  - Access to full 4GB of memory
  - Memory protection features
  - 32-bit registers and operations
  - Support for multitasking and paging
  - Required for modern OS development

### GDT (Global Descriptor Table)
- Data structure required for Protected Mode
- Defines memory segments (code, data, stack)
- Each entry is 8 bytes and contains:
  - Base address: Where segment starts in memory
  - Limit: Size of the segment
  - Access byte: Defines privileges and permissions
  - Flags: Controls granularity and other properties
- Minimum of 3 entries: null descriptor, code segment, data segment

### IDT (Interrupt Descriptor Table)
- Maps interrupt numbers to handler functions
- Used by CPU to find the appropriate code when an interrupt occurs
- Essential for handling hardware events (keyboard, timer, etc.)
- Each entry points to an Interrupt Service Routine (ISR)

### ISR (Interrupt Service Routine)
- Functions that handle specific interrupts
- Save CPU state, process the interrupt, restore state, and return
- Connected to entries in the IDT
- In MythOS: Currently handling keyboard interrupts (IRQ1)

### Kernel
- Core of the operating system
- Manages system resources and provides services
- In MythOS: Initialized by bootloader, sets up IDT and ISRs
- Entry point is `kernel_main()` function

## Boot Process Flow

1. **BIOS Initialization**:
   - CPU starts executing from fixed address
   - BIOS performs hardware checks (POST)
   - BIOS locates bootable device

2. **Bootloader Execution**:
   - BIOS loads bootloader at 0x7C00
   - Bootloader sets up GDT
   - Enables Protected Mode by setting PE bit in CR0
   - Performs far jump to flush pipeline
   - Initializes segment registers
   - Jumps to kernel entry point

3. **Kernel Initialization**:
   - `kernel_main()` begins execution
   - Displays "MythOS is alive!" on screen
   - Sets up IDT for interrupt handling
   - Installs ISRs for keyboard input
   - Enables interrupts and initializes PIC
   - Enters infinite loop to keep system running

## File Descriptions

### `bootloader.asm`
- Contains code executed directly by BIOS
- Sets up GDT and switches to Protected Mode
- Jumps to kernel entry point
- Must fit within 512 bytes and end with 0xAA55

### `gdt.asm`
- Defines the Global Descriptor Table
- Contains null descriptor, code segment, and data segment
- Required for Protected Mode memory segmentation

### `kernel.cpp`
- Contains `kernel_main()` function (entry point)
- Writes text to video memory (0xB8000)
- Sets up interrupt handling
- Main OS logic will be expanded here

### `idt.cpp` and `include/idt.h`
- Defines the Interrupt Descriptor Table structure
- Contains functions to set up IDT entries
- Maps interrupts to their handlers

### `isr.asm` and `include/isr.h`
- Contains low-level interrupt handler functions
- Saves/restores CPU state during interrupt processing
- Bridges assembly-level and C++ interrupt handling

### `load_idt.asm`
- Uses `lidt` instruction to load IDT into CPU
- Bridge between C++ and assembly for IDT loading

### `linker.ld`
- Controls memory layout of the kernel
- Ensures kernel is loaded at correct address (0x1000)
- Defines sections for code (.text), data (.data), and uninitialized data (.bss)

### `Makefile`
- Automates build process
- Compiles assembly and C++ files
- Links everything together
- Creates bootable binary
- Provides commands to run OS in QEMU

## Important Memory Addresses

- **0x7C00**: Where BIOS loads the bootloader
- **0x1000**: Where our kernel is loaded
- **0xB8000**: VGA text mode memory (for screen output)
- **0x60**: Keyboard data port (for reading keypresses)

## Week 1 Achievements

- Set up development environment
- Created a bootable OS that enters Protected Mode
- Implemented a basic kernel in C++
- Set up interrupt handling infrastructure
- Displayed text on screen
- Began handling keyboard input

## Next Steps (Week 2)

- Enhance text output with colors and formatting
- Implement full keyboard input handling
- Begin memory management implementation
- Create a command-line interface
- Add basic system commands

## Debugging Tips

- Use `make clean` before rebuilding to ensure fresh compilation
- Check bootloader size (must be ≤ 512 bytes)
- Verify boot signature (0xAA55) is present
- Use QEMU with debugging options for detailed inspection
- Remember that Protected Mode requires proper GDT setup
