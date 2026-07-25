#include "chip8.hpp"
#include <cstdint>

void Chip_8::opcode(uint16_t code) {
  uint16_t nnn = code & 0x0FFF;
  uint8_t a = code >> 12;
  uint8_t n = code & 0x000F;
  uint8_t x = (code >> 8) & 0x000F;
  uint8_t y = (code >> 4) & 0x000F;
  uint8_t kk = code & 0x00FF;

  switch (a) {
    case 0:

    case 1: //Jump to location nnn/address
      PC += nnn;

    case 2: //Call subroutine at nnn
      stack[SP] = PC;
      ++SP;
      PC = nnn;

    case 3: //Skip next instruction if Vx == kk
      if(registers[x] != kk) {
        PC += 2;
      }

    case 4: //Skip next instruction if Vx != kk
      if(registers[x] == kk) {
        PC += 2;
      }

    case 5: //Skip instruction if Vx == Vy
      if (registers[x] == registers[y]) {
        PC += 2;
      }

    case 6: //Set Vx == kk
      registers[x] = kk;

    case 7: //Set Vx = Vx + kk
      registers[x] += kk;

    case 8: //Set Vx == Vy
      switch (n) {
        case 0:
          registers[x] = registers[y];
        case 1:
          registers[x] = registers[x] | registers[y];
        case 2:
          registers[x] = registers[x] & registers[y];
      }
  }
}
