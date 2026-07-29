#ifndef CHIP_8
#define CHIP_8

// Declaring libraries
#include <cstdint>

class Chip_8 {
  public:
    // Constructor
    Chip_8();

    // Declaring Chip_8's instruction
    void opcode(uint16_t code);

  private:
    // Declaring registers
    uint8_t V[16];
    uint16_t I; // Storing memory addresses

    // Declaring program counter and stack pointer
    uint16_t pc; // The program begins at 0x200
    uint8_t sp;

    // Declaring memory
    uint8_t memory[4096];

    // Declaring display
    uint8_t video[256];
};

#endif // CHIP_8
