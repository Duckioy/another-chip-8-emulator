#include <cstdint>
#include <stdio.h>

typedef struct {
    uint8_t memory[512];
    uint8_t v[16];

    uint16_t pc;
} Chip;

int main() {
    Chip chip = {.pc = 0x200};

    uint8_t rom[] = {0x00, 0x0ee};

    printf("Hello world");
}
