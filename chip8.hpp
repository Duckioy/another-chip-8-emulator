#ifndef CHIP_8_
#define CHIP_8_

#include <chrono>
#include <cstdint>
#include <random>
#include <stdint.h>

class Chip_8 {
  uint8_t registers[15];
  uint8_t memory[4096];

  uint8_t VF = 0;

  uint16_t I; // Storing memory addresses

  uint16_t stack[16]; // Amount of stack

  // Program counter and stack pointer
  uint16_t PC = 0;
  uint16_t SP = 0;

  // Displayed Frame
  uint32_t video[64 * 32];

public:
  Chip_8();
  // Load ROM to the memory
  void LoadROM(char const *filename);

  // Chip-8 Instructions
  void opcode(uint16_t code);
};

#endif // CHIP_8_
