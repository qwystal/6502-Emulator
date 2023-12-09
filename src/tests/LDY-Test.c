#include "../emulator/includes.h"

void testLDYFlags(CPU *cpu, CPU *cpucopy, Memory *mem)
{
    if (/*cpu->A == 0 && cpu->Z || */!(cpu->Y == 0) && !cpu->Z)
    {
        _log("Zero Flag set succeeded.", 0);
    }
    else
    {
        _log("Zero Flag set failed.", 1);
    }

    if (/*getBitOfByte(cpu->A, 7) == 1 && cpu->N || */!(getBitOfByte(cpu->Y, 7) == 1) && !cpu->N)
    {
        _log("Negative Flag set succeeded.", 0);
    }
    else
    {
        _log("Negative Flag set failed.", 1);
    }

    printf("\n");
}

void testLDY()
{
    CPU cpu;
    CPU cpucopy;
    Memory mem;
    errors = 0;

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDY_IM; // Load byte from memory into Accumulator immediatly
    mem.Data[0xFFFD] = 0x01;
    execute(CYC_LDY_IM, &mem, &cpu);
    if (!cpu.Y)
    {
        _log("INS_LDY_IM failed.", 1);
    }
    else
    {
        _log("INS_LDY_IM succeeded.", 0);
    }
    testLDYFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDY_ZP; // Load byte from memory into Accumulator from pointer
    mem.Data[0xFFFD] = 0x42;
    mem.Data[0x0042] = 0x01;
    execute(CYC_LDY_ZP, &mem, &cpu);
    if (!cpu.Y)
    {
        _log("INS_LDY_ZP failed.", 1);
    }
    else
    {
        _log("INS_LDY_ZP succeeded.", 0);
    }
    testLDYFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDY_ZPX; // Load byte from memory into Accumulator from pointer + X
    cpu.X = 0x01;
    mem.Data[0xFFFD] = 0x42;
    mem.Data[0x0043] = 0x01;
    execute(CYC_LDY_ZPX, &mem, &cpu);
    if (!cpu.Y)
    {
        _log("INS_LDY_ZPX failed.", 1);
    }
    else
    {
        _log("INS_LDY_ZPX succeeded.", 0);
    }
    testLDYFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDY_AB; // Load byte from memory into Accumulator from absolute address (2 byte)
    mem.Data[0xFFFD] = 0x12;
    mem.Data[0xFFFE] = 0x34;
    mem.Data[0x3412] = 0x42;
    execute(CYC_LDY_AB, &mem, &cpu);
    if (!cpu.Y)
    {
        _log("INS_LDY_AB failed.", 1);
    }
    else
    {
        _log("INS_LDY_AB succeeded.", 0);
    }
    testLDYFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDY_ABX; // Load byte from memory into Accumulator from absolute address (2 byte) + X
    mem.Data[0xFFFD] = 0x12;
    mem.Data[0xFFFE] = 0x34;
    mem.Data[0x3511] = 0x42;
    cpu.X = 0xFF;
    execute(CYC_LDY_ABX_PC, &mem, &cpu);
    if (!cpu.Y)
    {
        _log("INS_LDY_ABX failed.", 1);
    }
    else
    {
        _log("INS_LDY_ABX succeeded.", 0);
    }

    testLDYFlags(&cpu, &cpucopy, &mem);
    printf("Test LDY finished with %d error(s).\n\n\n", errors);
}
