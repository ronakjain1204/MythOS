extern "C" void kernel_main() {
    char* video_memory = (char*) 0xB8000;

    const char* message = "MythOS is alive!";
    int i = 0;

    while (message[i] != '\0') {
        video_memory[i * 2] = message[i];     // Character
        video_memory[i * 2 + 1] = 0x0F;        // White on black
        i++;
    }

    while (1); // Infinite loop to hang
}
