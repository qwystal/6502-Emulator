#include "../emulator/includes.h"

void testLDXFlags(CPU *cpu, CPU *cpucopy, Memory *mem)
{
    if (/*cpu->A == 0 && cpu->Z || */!(cpu->X == 0) && !cpu->Z)
    {
        _log("Zero Flag set succeeded.", 0);
    }
    else
    {
        _log("Zero Flag set failed.", 1);
    }

    if (/*getBitOfByte(cpu->A, 7) == 1 && cpu->N || */!(getBitOfByte(cpu->X, 7) == 1) && !cpu->N)
    {
        _log("Negative Flag set succeeded.", 0);
    }
    else
    {
        _log("Negative Flag set failed.", 1);
    }

    printf("\n");
}

void testLDX()
{
    CPU cpu;
    CPU cpucopy;
    Memory mem;
    errors = 0;

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDX_IM; // Load byte from memory into Accumulator immediatly
    mem.Data[0xFFFD] = 0x01;
    execute(CYC_LDX_IM, &mem, &cpu);
    if (!cpu.X)
    {
        _log("INS_LDX_IM failed.", 1);
    }
    else
    {
        _log("INS_LDX_IM succeeded.", 0);
    }
    testLDXFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDX_ZP; // Load byte from memory into Accumulator from pointer
    mem.Data[0xFFFD] = 0x42;
    mem.Data[0x0042] = 0x01;
    execute(CYC_LDX_ZP, &mem, &cpu);
    if (!cpu.X)
    {
        _log("INS_LDX_ZP failed.", 1);
    }
    else
    {
        _log("INS_LDX_ZP succeeded.", 0);
    }
    testLDXFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDX_ZPY; // Load byte from memory into Accumulator from pointer + X
    cpu.Y = 0x01;
    mem.Data[0xFFFD] = 0x42;
    mem.Data[0x0043] = 0x01;
    execute(CYC_LDX_ZPY, &mem, &cpu);
    if (!cpu.X)
    {
        _log("INS_LDX_ZPY failed.", 1);
    }
    else
    {
        _log("INS_LDX_ZPY succeeded.", 0);
    }
    testLDXFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDX_AB; // Load byte from memory into Accumulator from absolute address (2 byte)
    mem.Data[0xFFFD] = 0x12;
    mem.Data[0xFFFE] = 0x34;
    mem.Data[0x3412] = 0x42;
    execute(CYC_LDX_AB, &mem, &cpu);
    if (!cpu.X)
    {
        _log("INS_LDX_AB failed.", 1);
    }
    else
    {
        _log("INS_LDX_AB succeeded.", 0);
    }
    testLDXFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDX_ABY; // Load byte from memory into Accumulator from absolute address (2 byte) + X
    mem.Data[0xFFFD] = 0x12;
    mem.Data[0xFFFE] = 0x34;
    mem.Data[0x3511] = 0x42;
    cpu.Y = 0xFF;
    execute(CYC_LDX_ABY_PC, &mem, &cpu);
    if (!cpu.X)
    {
        _log("INS_LDX_ABY failed.", 1);
    }
    else
    {
        _log("INS_LDX_ABY succeeded.", 0);
    }
    testLDXFlags(&cpu, &cpucopy, &mem);
    printf("Test LDX finished with %d error(s).\n\n\n", errors);
}
