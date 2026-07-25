#include "chip8.hpp"
#include <cstdint>
#include <cstdlib>

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
      break;

    case 2: //Call subroutine at nnn
      stack[SP] = PC;
      ++SP;
      PC = nnn;
      break;

    case 3: //Skip next instruction if Vx == kk
      if(registers[x] != kk) {
        PC += 2;
      }
      break;

    case 4: //Skip next instruction if Vx != kk
      if(registers[x] == kk) {
        PC += 2;
      }
      break;

    case 5: //Skip instruction if Vx == Vy
      if (registers[x] == registers[y]) {
        PC += 2;
      }
      break;

    case 6: //Set Vx == kk
      registers[x] = kk;
      break;

    case 7: //Set Vx = Vx + kk
      registers[x] += kk;
      break;

    case 8:
      switch (n) {
        case 0: //Set Vx == Vy
          registers[x] = registers[y];
          break;
        case 1:
          registers[x] = registers[x] | registers[y];
          break;
        case 2:
          registers[x] = registers[x] & registers[y];
          break;
        case 3:
          registers[x] = registers[x] ^ registers[y];
          break;
        case 4: {
          uint8_t total = registers[x] + registers[y];
          if(total > 0xFF) {
            VF = 1;
          } else {
            VF = 0;
          }
          registers[x] = total;
          break;
        }
        case 5: {
          uint8_t subtract = registers[x] - registers[y];
          if(registers[x] > registers[y]) {
            VF = 1;
          } else {
            VF = 0;
          }
          registers[x] = registers[y] - registers[x];
          break;
        }
        case 6: {
          if(x & 0x0001) {
            VF = 1;
          } else {
            VF = 0;
          }
          registers[x] /= 2;
          break;
        }
        case 7: {
          if (registers[x] < registers[y]) {
            VF = 1;
          } else {
            VF = 0;
          }
          registers[x] -= registers[y];
          break;
        }
        case 14: {
          VF = 1;
          registers[x] <<= 1;
          break;
        }
      }
      case 9:
        if (registers[x] != registers[y]) {
          PC += 2;
        }
        break;
      case 10:
        I = nnn;
        break;
      case 11:
        PC = nnn + registers[0];
        break;
      case 12: {
        uint8_t random_byte = rand();
        registers[x] = random_byte & kk;
        break;
      }
  }
}
