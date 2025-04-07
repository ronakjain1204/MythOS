# **🧠 Deep Dive: Understanding the Global Descriptor Table (GDT)**

Hey everyone! Ronak here. Today, we’re going to explore the **Global Descriptor Table (GDT)**—a critical component of switching to **Protected Mode** in OS development. Don’t worry if this sounds intimidating; I’ll break it down step-by-step so we can all understand how it works and why it’s essential. Let’s dive in! 😊  

---

## **🔧 What is the GDT?**
The **Global Descriptor Table (GDT)** is a data structure used by the CPU in **Protected Mode** to define the layout of memory segments. Think of it as the CPU’s "rulebook" for accessing memory safely and efficiently.  

### **Real Mode vs. Protected Mode**
- **Real Mode (16-bit):**  
  Uses simple `segment:offset` addressing (e.g., `CS:IP`, `DS:SI`).  
  Limited to **1MB of memory** and lacks security features.  

- **Protected Mode (32-bit):**  
  Uses the GDT to define memory segments with **permissions**, **base addresses**, and **limits**.  
  Enables access to **4GB of memory** and advanced features like memory protection.  

**Why do we need the GDT?**  
When switching to Protected Mode, the CPU **demands a GDT**. Without it, the CPU will crash or reboot (triple fault). The GDT:  
- Defines memory segments (code, data, stack).  
- Removes the 1MB memory limit of Real Mode.  
- Provides isolation between kernel and user space (in advanced OSes).  
- Protects memory regions (read-only, executable, etc.).  

---

## **🧰 What’s Inside the GDT?**
The GDT is an array of **8-byte entries** called **segment descriptors**. Each entry defines a memory segment with the following fields:  

| Field           | Bits  | Purpose                                                                 |
|-----------------|-------|-------------------------------------------------------------------------|
| **Base Address** | 32    | Start address of the segment.                                          |
| **Segment Limit**| 20    | Size of the segment.                                                   |
| **Access Byte**  | 8     | Type of segment (code/data), privilege level (kernel/user), and flags. |
| **Flags**        | 4     | Granularity (4KB blocks), 32/64-bit mode.                              |

---

## **🧾 A Typical GDT Structure**
Most GDTs have 3 entries:  

| Index | Selector | Segment Purpose          |
|-------|----------|--------------------------|
| 0     | `0x00`   | **Null Segment** (required!) |
| 1     | `0x08`   | **Code Segment** (executable, kernel-level) |
| 2     | `0x10`   | **Data Segment** (writable, kernel-level) |

### **Selectors Explained**
- **Selector = Index × 8** (e.g., `0x08 = 1 × 8` for the code segment).  
- The selector is passed to segment registers (e.g., `CS`, `DS`) to reference the GDT entry.  

---

## **🔍 Decoding a GDT Entry (Code Segment Example)**
Let’s dissect the **code segment descriptor** from our GDT:  
```nasm
dq 0x00CF9A000000FFFF   ; Code segment descriptor
```

### **Breaking Down the Hex Value**
1. **Base Address**: `0x00000000` (starts at memory address 0).  
2. **Segment Limit**: `0xFFFFF` (1MB × 4KB granularity = 4GB total).  
3. **Access Byte**: `0x9A` (binary: `10011010`):  

| Bit | Name      | Value | Meaning                          |
|-----|-----------|-------|----------------------------------|
| 7   | Present   | 1     | Segment is in memory.            |
| 6-5 | DPL       | 00    | Kernel privilege level (Ring 0). |
| 4   | S         | 1     | Code/Data segment (not system).  |
| 3-0 | Type      | 1010  | Code segment, readable, non-conforming. |

4. **Flags**: `0xC` (binary: `1100`):  
   - **G (Granularity)**: `1` → Multiply limit by 4KB.  
   - **D (32-bit)**: `1` → 32-bit segment.  
   - **L (64-bit)**: `0` → Not used here.  
   - **AVL**: `0` → Ignored by the CPU.  

---

## **📜 The Full GDT Code (gdt.asm)**
Here’s the GDT we use in MythOS:  
```nasm
gdt_start:
    dq 0x0000000000000000         ; Null segment (mandatory)
    dq 0x00CF9A000000FFFF         ; Code segment (base=0, limit=4GB)
    dq 0x00CF92000000FFFF         ; Data segment (base=0, limit=4GB)
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1    ; Size of GDT - 1
    dd gdt_start                  ; Address of GDT
```

### **Line-by-Line Explanation**
1. **`gdt_start`**: Label marking the start of the GDT.  
2. **Null Segment**: First entry (required) set to zeros.  
3. **Code Segment**:  
   - `0x00CF9A000000FFFF`: 32-bit code segment spanning 4GB.  
4. **Data Segment**:  
   - `0x00CF92000000FFFF`: 32-bit data segment spanning 4GB.  
5. **`gdt_descriptor`**:  
   - **`dw gdt_end - gdt_start - 1`**: Size of the GDT (minus 1).  
   - **`dd gdt_start`**: Memory address of the GDT.  

---

## **🔥 Recap: How the GDT Enables Protected Mode**
To switch to Protected Mode, you must:  
1. **Define the GDT** with code/data segments.  
2. **Load the GDT** using `lgdt [gdt_descriptor]`.  
3. **Set the PE bit** in the `CR0` register to enable Protected Mode.  
4. **Far jump** to a code segment selector (e.g., `jmp 0x08:init_pm`).  
5. **Update segment registers** (DS, ES, etc.) to point to the data segment.  

---

## **💡 Why This Matters for MythOS**
The GDT is the foundation for:  
- **Memory Protection**: Prevent unauthorized access to critical regions.  
- **Multitasking**: Isolate processes into separate segments.  
- **Modern OS Features**: Paging, user/kernel mode separation, and more.  

Without the GDT, we’d be stuck in the 1980s with Real Mode’s limitations!  

---

Let me know if you have questions or want to dive deeper into any part of the GDT. Let’s keep learning together! 😊  

**Next up**: We’ll use this GDT to jump into 32-bit code and start building our kernel! 🚀
