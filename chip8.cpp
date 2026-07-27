#include "chip8.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <random>

// Declaring the start address
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

    // Load the ROM contents into the chip_8 Memory, starting at 0x200 and run from it
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
  //Defining the instructions' keys
  // uint16_t nnn = code & 0x0FFF;
  // uint8_t a = code >> 12;
  // uint8_t n = code & 0x000F;
  // uint8_t x = (code >> 8) & 0x000F;
  // uint8_t y = (code >> 4) & 0x000F;
  // uint8_t kk = code & 0x00FF;

  switch (code >> 12) {
    case 0: {
      switch (code & 0x00FF) {
        //Clear the display.
        case 0xE0:
          memset(video, 0, sizeof(video));
          break;

        // The interpreter sets the program counter 
        // to the address at the top of the stack, 
        // then subtracts 1 from the stack pointer.
        case 0xEE:
          --SP;
          PC = stack[SP];
          break;
      }
    }

    //Jump to location nnn/address
    case 1:
      PC += (code & 0x0FFF);
      break;

    //Call subroutine at nnn
    case 2:
      stack[SP] = PC;
      ++SP;
      PC = (code & 0x0FFF);
      break;

    //Skip next instruction if Vx == kk
    case 3:
      if(registers[(code >> 8) & 0x000F] != (code & 0x00FF)) {
        PC += 2;
      }
      break;

    //Skip next instruction if Vx != kk
    case 4:
      if(registers[(code >> 8) & 0x000F] == (code & 0x00FF)) {
        PC += 2;
      }
      break;

    //Skip instruction if Vx == Vy
    case 5:
      if (registers[(code >> 8) & 0x000F] == registers[(code >> 4) & 0x000F]) {
        PC += 2;
      }
      break;
    //The interpreter puts the value kk into register Vx.
    case 6:
      registers[(code >> 8) & 0x000F] = (code & 0x00FF);
      break;

   //Adds the value kk to the value of register Vx, then stores the result in Vx.
    case 7:
      registers[(code >> 8) & 0x000F] += (code & 0x00FF);
      break;

    case 8:
      switch (code & 0x000F) {
        //Stores the value of register Vy in register Vx.
        case 0:
          registers[(code >> 8) & 0x000F] = registers[(code >> 4) & 0x000F];
          break;

        //Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. 
        //A bitwise OR compares the corrseponding bits from two values , 
        //and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0. 
        case 1:
          registers[(code >> 8) & 0x000F] |= registers[(code >> 4) & 0x000F];
          break;

        //Performs a bitwise AND on the values of Vx and Vy, 
        //then stores the result in Vx. A bitwise AND compares the corrseponding 
        //bits from two values, and if both bits are 1, then the same bit in the 
        //result is also 1. Otherwise, it is 0. 
        case 2:
          registers[(code >> 8) & 0x000F] &= registers[(code >> 4) & 0x000F];
          break;

        //Performs a bitwise exclusive OR on the values of Vx and Vy, 
        //then stores the result in Vx. An exclusive OR compares the corrseponding 
        //bits from two values, and if the bits are not both the same, then the corresponding 
        //bit in the result is set to 1. Otherwise, it is 0.
        case 3:
          registers[(code >> 8) & 0x000F] ^= registers[(code >> 4) & 0x000F];
          break;

        //The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) 
        //VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
        case 4: {
          uint8_t total = registers[(code >> 8) & 0x000F] + registers[(code >> 4) & 0x000F];
          if(total > 0xFF) {
            VF = 1;
          } else {
            VF = 0;
          }
          registers[(code >> 8) & 0x000F] = total;
          break;
        }

        // If Vx > Vy, then VF is set to 1, otherwise 0. 
        // Then Vy is subtracted from Vx, and the results stored in Vx.
        case 5: {
          uint8_t subtract = registers[(code >> 8) & 0x000F] - registers[(code >> 4) & 0x000F];
          if(registers[(code >> 8) & 0x000F] > registers[(code >> 4) & 0x000F]) {
            VF = 1;
          } else {
            VF = 0;
          }
          registers[(code >> 8) & 0x000F] = registers[(code >> 4) & 0x000F] - registers[(code >> 8) & 0x000F];
          break;
        }

        // If the least-significant bit of Vx is 1, 
        // then VF is set to 1, otherwise 0. Then Vx is divided by 2.
        case 6: {
          if(((code >> 8) & 0x000F) & 0x0001) {
            VF = 1;
          } else {
            VF = 0;
          }
          registers[(code >> 8) & 0x000F] /= 2;
          break;
        }

        // If Vy > Vx, then VF is set to 1, otherwise 0. 
        // Then Vx is subtracted from Vy, and the results stored in Vx.
        case 7: {
          if (registers[(code >> 8) & 0x000F] < registers[(code >> 4) & 0x000F]) {
            VF = 1;
          } else {
            VF = 0;
          }
          registers[(code >> 8) & 0x000F] -= registers[(code >> 4) & 0x000F];
          break;
        }

        // If the most-significant bit of Vx is 1, 
        // then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
        case 14: {
          VF = 1;
          registers[(code >> 8) & 0x000F] <<= 1;
          break;
        }
      }

      // The values of Vx and Vy are compared, 
      // and if they are not equal, the program counter is increased by 2.
      case 9:
        if (registers[(code >> 8) & 0x000F] != registers[(code >> 4) & 0x000F]) {
          PC += 2;
        }
        break;

      // The value of register I is set to nnn.
      case 10:
        I = (code & 0x0FFF);
        break;

      // The program counter is set to nnn plus the value of V0.
      case 11:
        PC = (code & 0x0FFF) + registers[0];
        break;

      // The interpreter generates a random number from 0 to 255, 
      // which is then ANDed with the value kk. The results are stored 
      // in Vx. See instruction 8xy2 for more information on AND.
      case 12: {
        std::default_random_engine randGen(std::random_device{}());
        std::uniform_int_distribution<uint8_t> randByte(0, 255);
        registers[(code >> 8) & 0x000F] = randByte(randGen) & (code & 0x00FF);
        break;
      }
  }
}
