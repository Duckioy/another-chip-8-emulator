#include "Chip_8.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <rom>\n";
        return 1;
    }

    Chip_8 chip8;
    chip8.loadROM(argv[1]);

    // CPU loop
    while (true) {
        chip8.cycle();
    }
}
