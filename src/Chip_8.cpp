#include "Chip_8.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ostream>

const uint8_t fontset[80]{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80, // F
};

void Chip_8::loadROM(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Cannot open ROM\n";
        return;
    }

    file.read(reinterpret_cast<char *>(&memory[0x200]), sizeof(memory) - 0x200);
}

Chip_8::Chip_8() {
    pc = 0x0200;
    sp = 0x00;
    I = 0x0000;
    memset(video, 0, sizeof(video));
    memset(memory, 0, sizeof(memory));
}

void Chip_8::cycle() {
    uint16_t code = (memory[pc] << 8) | memory[pc + 1];

    pc += 2;

    opcode(code);
}

void Chip_8::opcode(uint16_t code) {
    switch (code >> 12) {
    case 0x0: {
        switch (code & 0x0FFF) {

        // Clear the (deez nuts) screen
        case 0x00E0:
            for (int i = 0; i < 32; ++i) {
                std::cout << "░";
                for (int j = 0; j < 64; ++j) {
                    std::cout << "░";
                }
                std::cout << "\n";
            }
            std::cout << std::endl;
            memset(video, 0, sizeof(video));
            break;
        }
        break;
    }

    // Jump to location nnn.
    // The interpreter sets the program counter to nnn.
    case 0x1: {
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
        std::cout << I << std::endl;
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

        uint8_t xPos = V[x];
        uint8_t yPos = V[y];
        V[15] = 0;

        for (int i = 0; i < n; i++) {

            uint8_t spriteByte = memory[I + i];

            for (int j = 0; j < 8; j++) {

                if (spriteByte & (0x80 >> j)) {

                    uint8_t screenX = (xPos + j) % 64;
                    uint8_t screenY = (yPos + i) % 32;

                    int pixelIndex = screenY * 64 + screenX;

                    int byteIndex = pixelIndex / 8;
                    int bitIndex = pixelIndex % 8;

                    if (video[byteIndex] & (0x80 >> bitIndex)) {
                        V[0xF] = 1;
                    }

                    video[byteIndex] ^= (0x80 >> bitIndex);
                }
            }
            render();
        }
        break;
    }
    default:
        std::cout << "Unknown opcode: " << std::hex << code << '\n';
        break;
    }
}

void Chip_8::render() {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            int pixelIndex = (y * 64) + x;
            int byteIndex = pixelIndex >> 3;
            int bitIndex = pixelIndex % 8;

            bool on = video[byteIndex] & (0x80 >> bitIndex);

            std::cout << (on ? "█" : "░");
        }
        std::cout << '\n';
    }
}
