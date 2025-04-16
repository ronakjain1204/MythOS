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
