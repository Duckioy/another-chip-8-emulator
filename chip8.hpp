#ifndef CHIP_8_
#define CHIP_8_

#include <cstdint>
#include <stdint.h>

class Chip_8 {
private:
  uint8_t registers[15];
  uint8_t memory[4096];

  uint8_t VF;

  uint16_t I; // Storing memory addresses

  uint16_t stack[16]; // Amount of stack

  // Program counter and stack counter
  uint16_t PC = 0;
  uint16_t SP = 0;

  uint32_t video[64 * 32];

  void opcode(uint16_t code);
};

#endif // CHIP_8_
