#include "chip8.hpp"
#include <cstdint>
#include <cstdlib>
#include <fstream>

const unsigned int START_ADDRESS = 0x200;

void Chip_8::LoadROM(char const* filename) {
  // Open the file as a stream of binary and move the file pointer to the end
  std::ifstream file(filename, std::ios::binary | std::ios::ate);
  if (file.is_open()) {

    // Get size of the file and allocate the buffer to hold the content
    std::streampos size = file.tellg();
    char* buffer = new char[size];

    // Go back to the beginning of the file and fill the buffer
    file.seekg(0, std::ios::beg);
    file.read(buffer, size);
    file.close();

    // Load the ROM contents into the chip_8 Memory, starting at 0x200
    for (long i = 0; i < size; ++i) {
      memory[START_ADDRESS + i] = buffer[i];
    }

    // Free the buffer
    delete[] buffer;
  }
}
const unsigned int FONTSET_SIZE = 80;

uint8_t fontset[FONTSET_SIZE] = 
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 1
    0x20, 0x60, 0x20, 0x20, 0x70, // 2
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 3
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 4
    0x90, 0x90, 0xF0, 0x10, 0x10, // 5
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 6
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 7
    0xF0, 0x10, 0x20, 0x40, 0x40, // 8
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

const unsigned int FONTSET_START_ADDRESS = 0x50;

Chip_8::Chip_8() {
  // Initialize the program counter
  PC = START_ADDRESS;

  // Load fonts into memory
  for (unsigned int i = 0; i < FONTSET_SIZE; ++i) {
    memory[FONTSET_START_ADDRESS + i] = fontset[i];
  }
}

void Chip_8::opcode(uint16_t code) {
  //TODO:
  //Need to overhaul this to avoid bloat when loading the code in
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
