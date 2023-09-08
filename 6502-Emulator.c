#include "6502_emulator.h"

int main()
{
    CPU cpu;
    Memory mem;

    start(&cpu, &mem);
    mem.Data[0xFFFC] = INS_LDA_ZP;
    mem.Data[0xFFFD] = 0x34;
    mem.Data[0x0034] = 0x42;
    execute(3, &mem, &cpu);
    printf("contains %c", cpu.A);

    return 0;
}