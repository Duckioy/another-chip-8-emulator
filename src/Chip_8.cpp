#include "Chip_8.hpp"
#include <cstdint>
#include <iostream>
#include <ostream>

Chip_8::Chip_8() {
    std::cout << "Holy Shit! Something is fuckin run..." << std::endl;
    pc = 0x0200;
    sp = 0x00;
    I = 0x0000;
}

void Chip_8::opcode(uint16_t code) {
    std::cout << "Welcome to this retared instructions for Chip_8 with nothing works at all" << std::endl;
    std::cout << (code >> 12) << std::endl << (code & 0x00FF) << "\n";
    switch (code >> 12) {
    case 0x0: {
        std::cout << "It is zero so it is nothing here, I swear" << std::endl;
        switch (code & 0x0FFF) {

        // Clear the (deez nuts) screen
        case 0x00E0:
            std::cout << "Yes! I thank you to actually clean me up!" << std::endl;
            for (int i = 0; i < 32; ++i) {
                std::cout << "█";
                for (int j = 0; j < 64; ++j) {
                    std::cout << "█";
                }
                std::cout << "\n";
            }
            std::cout << std::endl;
            for (int i = 0; i < sizeof(video); ++i) {
                video[i] = 0x00;
                break;
            }
        }
        break;
    }
    // Jump to location nnn.
    // The interpreter sets the program counter to nnn.
    case 0x1: {
        std::cout << "Yes~ Jump on this position tehe~" << std::endl;
        pc = code & 0x0FFF;
        break;
    }

    // Set Vx = kk.
    // The interpreter puts the value kk into register Vx.
    case 0x6: {
        uint8_t x = (code >> 8) & 0x000F;
        uint8_t kk = code & 0x00FF;
        V[x] = kk;
        break;
    }

    // Set Vx = Vx + kk.
    // Adds the value kk to the value of register Vx, then stores the result in Vx.
    case 0x7: {
        uint8_t x = (code >> 8) & 0x000F;
        uint8_t kk = code & 0x00FF;
        V[x] += kk;
        break;
    }

    // Set I = nnn.
    // The value of register I is set to nnn.
    case 0xA: {
        uint16_t nnn = code & 0x0FFF;
        I = nnn;
        break;
    }

    // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
    // The interpreter reads n bytes from memory, starting at the address stored in I.
    // These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
    // Sprites are XORed onto the existing screen.
    // If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
    // If the sprite is positioned so part of it is outside the coordinates of the display,
    // it wraps around to the opposite side of the screen.
    case 0xD: {
        uint8_t x = (code >> 8) & 0x000F;
        uint8_t y = (code >> 4) & 0x000F;
        uint8_t n = code & 0x000F;
        pc = I;
    }
    }
}
