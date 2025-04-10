all: build/bootloader.bin

build/bootloader.bin: bootloader.asm kernel.cpp linker.ld
	mkdir -p build
	nasm -f bin bootloader.asm -o build/bootloader_stage1.bin
	i686-elf-g++ -ffreestanding -m32 -c kernel.cpp -o build/kernel.o
	i686-elf-ld -m elf_i386 -T linker.ld -o build/kernel.bin build/kernel.o
	cat build/bootloader_stage1.bin build/kernel.bin > build/bootloader.bin

run:
	qemu-system-i386 -drive format=raw,file=build/bootloader.bin

clean:
	rm -rf build
