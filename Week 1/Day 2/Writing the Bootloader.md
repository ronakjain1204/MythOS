# **📌 Week 1 - Day 2: Writing the Bootloader (Step-by-Step Guide)**  

## 🎯 **What We Will Achieve Today**
By the end of today, we will:  
✅ Write a **basic bootloader** in Assembly.  
✅ Understand how **BIOS loads the bootloader**.  
✅ Learn how to display text on the screen.  
✅ Test our bootloader using **QEMU** (without rebooting our computer).  

---

# **📌 Step 1: What Is a Bootloader?**  

### 🔹 **Why Do We Need a Bootloader?**  
- When we turn on our computer, the BIOS executes a **Power-On Self-Test (POST)** and looks for a **bootable device** (HDD, SSD, USB)[4][5].  
- The first **512 bytes** of a bootable device contain a special program called the **bootloader**.  
- The **bootloader's job** is to load the operating system into memory and start execution.  

📌 **Think of the bootloader as the ignition key of our OS—it starts everything!**  

---

# **📌 Step 2: Writing Our First Bootloader in Assembly**

### **📝 Create `bootloader.asm` inside the `MythOS/` folder**  
Let's open a terminal, go inside our project folder, and create the file:

```sh
cd MythOS
touch bootloader.asm
```

Then, open it in a text editor like **VS Code** or **Vim**:

```sh
code bootloader.asm
```
*(We can use `nano bootloader.asm` if we prefer terminal editors.)*

---

### **📌 Step 3: Write the Bootloader Code**
Now, let's copy and paste the following code into `bootloader.asm`:

```assembly
[BITS 16]     ; Tell assembler we are writing 16-bit code (Real Mode)
[ORG 0x7C00]  ; BIOS loads the bootloader at memory address 0x7C00

start:
    mov si, message     ; Load address of message into SI register
    call print_string   ; Call the function to print the message
    jmp $              ; Infinite loop to stop execution

print_string:
    lodsb              ; Load next character from SI into AL
    or al, al          ; Check if AL is zero (end of string)
    jz done            ; If zero, we are done
    mov ah, 0x0E       ; BIOS teletype function (prints a character)
    int 0x10           ; Call BIOS interrupt
    jmp print_string   ; Repeat for next character
done:
    ret

message db "Hello, MythOS!", 0  ; Define the message to print

times 510 - ($ - $$) db 0  ; Fill the rest of the boot sector with zeroes
dw 0xAA55  ; Boot sector signature (BIOS requires this to boot)
```

---

# **📌 Step 4: Understanding the Code**
Let's break it down **line by line** so we fully understand what's happening.

### **🔹 Setting Up the Bootloader**
```assembly
[BITS 16]     ; Tell the assembler that this is 16-bit code (Real Mode)
[ORG 0x7C00]  ; Tell the assembler that BIOS loads us at 0x7C00 in memory
```
📌 The CPU starts in **Real Mode (16-bit)** when powered on, so our bootloader **must** also be 16-bit[1][9].

### **🔹 Printing a String Using BIOS**
```assembly
mov si, message   ; Load the address of our message into register SI
call print_string ; Call the function that prints the message
jmp $             ; Infinite loop (halts execution)
```
📌 This loads the **address** of `"Hello, MythOS!"` into `SI` and calls `print_string` to display it.

### **🔹 Printing Each Character**
```assembly
print_string:
    lodsb              ; Load the next character of the string into AL
    or al, al          ; Check if AL is zero (end of string)
    jz done            ; If zero, stop printing
    mov ah, 0x0E       ; BIOS function: Print a character
    int 0x10           ; Call BIOS interrupt 0x10
    jmp print_string   ; Repeat for next character
done:
    ret
```
📌 This prints **one character at a time** using **BIOS interrupt 0x10**.

### **🔹 Defining the Message**
```assembly
message db "Hello, MythOS!", 0  ; Define the message to print
```
📌 This stores `"Hello, MythOS!"` as text. The `0` at the end signals the string's **end**.

### **🔹 Boot Sector Padding**
```assembly
times 510 - ($ - $$) db 0  ; Fill remaining bytes with zeros
dw 0xAA55  ; BIOS boot signature
```
📌 The **boot sector must be exactly 512 bytes**.  
📌 `0xAA55` tells BIOS: *"Yes, this is a valid bootloader!"*

---

# **📌 Step 5: Compiling and Running the Bootloader**
Now, let's turn our **Assembly code** into a binary file that can boot in QEMU.

### **🔹 Assemble the Code**
Let's run the following command to compile `bootloader.asm` into a binary file:
```sh
nasm -f bin bootloader.asm -o bootloader.bin
```
🔍 **What This Does:**  
- `nasm` → Calls the assembler.  
- `-f bin` → Outputs a raw binary file (not an executable).  
- `bootloader.asm` → Input file.  
- `-o bootloader.bin` → Output file.  

If successful, we will now have a file called **`bootloader.bin`** (512 bytes).

---

# **📌 Step 6: Testing in QEMU**
Now, let's run the bootloader in **QEMU**, a virtual machine emulator.

### **🔹 Run the Bootloader**
```sh
qemu-system-x86_64 -drive format=raw,file=bootloader.bin
```

🔍 **What This Does:**  
- `qemu-system-x86_64` → Launches a virtual x86 computer.  
- `-drive format=raw,file=bootloader.bin` → Tells QEMU to boot from `bootloader.bin`.  

🔥 **If everything is correct, we should see:**
```
Hello, MythOS!
```
displayed on the virtual screen.

---

# **✅ Summary of Day 2**
✔️ **Wrote a bootloader in Assembly.**  
✔️ **Learned how BIOS loads boot sectors.**  
✔️ **Understood how to print text using BIOS interrupts.**  
✔️ **Compiled and tested our bootloader in QEMU.**  

---

# **🚀 What's Next? (Day 3 Preview)**
📌 **Tomorrow (Day 3), we will:**  
1. Modify the bootloader to **switch to Protected Mode (32-bit).**  
2. Prepare for writing the **kernel in C++**.  
3. Learn about **memory segmentation and CPU modes**.  

🔥 **Get ready! We are about to move beyond Real Mode!**
