#include "../emulator/includes.h"

void testLDAFlags(CPU *cpu, CPU *cpucopy, Memory *mem)
{
    if (cpu->C == cpucopy->C)
    {
        _log("Carry Flag set succeeded.", 0);
    }
    else
    {
        _log("Carry Flag set failed.", 1);
    }    

    if (/*cpu->A == 0 && cpu->Z || */!(cpu->A == 0) && !cpu->Z)
    {
        _log("Zero Flag set succeeded.", 0);
    }
    else
    {
        _log("Zero Flag set failed.", 1);
    }

    if (cpu->I == cpucopy->I)
    {
        _log("Interrupt Disable set succeeded.", 0);
    }
    else
    {
        _log("Interrupt Disable set failed.", 1);
    }

    if (cpu->D == cpucopy->D)
    {
        _log("Decimal Mode Flag set succeeded.", 0);
    }
    else
    {
        _log("Decimal Mode Flag set failed.", 1);
    }

    if (cpu->B == cpucopy->B)
    {
        _log("Break Command set succeeded.", 0);
    }
    else
    {
        _log("Break Command set failed.", 1);
    }

    if (cpu->V == cpucopy->V)
    {
        _log("Overflow Flag set succeeded.", 0);
    }
    else
    {
        _log("Overflow Flag set failed.", 1);
    }

    if (/*getBitOfByte(cpu->A, 7) == 1 && cpu->N || */!(getBitOfByte(cpu->A, 7) == 1) && !cpu->N)
    {
        _log("Negative Flag set succeeded.", 0);
    }
    else
    {
        _log("Negative Flag set failed.", 1);
    }

    printf("\n");
}

void testLDA()
{
    CPU cpu;
    CPU cpucopy;
    Memory mem;

    errors = 0;

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDA_IM; // Load byte from memory into Accumulator immediatly
    mem.Data[0xFFFD] = 0x01;
    execute(CYC_LDA_IM, &mem, &cpu);
    !cpu.A ? _log("INS_LDA_IM failed.", 1) : _log("INS_LDA_IM succeeded.", 0);
    testLDAFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDA_ZP; // Load byte from memory into Accumulator from pointer
    mem.Data[0xFFFD] = 0x42;
    mem.Data[0x0042] = 0x01;
    execute(CYC_LDA_ZP, &mem, &cpu);
    !cpu.A ? _log("INS_LDA_ZP failed.", 1) : _log("INS_LDA_ZP succeeded.", 0);
    testLDAFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDA_ZPX; // Load byte from memory into Accumulator from pointer + X
    cpu.X = 0x01;
    mem.Data[0xFFFD] = 0x42;
    mem.Data[0x0043] = 0x01;
    execute(CYC_LDA_ZPX, &mem, &cpu);
    if (!cpu.A)
    {
        _log("INS_LDA_ZPX failed.", 1);
    }
    else
    {
        _log("INS_LDA_ZPX succeeded.", 0);
    }
    testLDAFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDA_AB; // Load byte from memory into Accumulator from absolute address (2 byte)
    mem.Data[0xFFFD] = 0x12;
    mem.Data[0xFFFE] = 0x34;
    mem.Data[0x3412] = 0x42;
    execute(CYC_LDA_AB, &mem, &cpu);
    if (!cpu.A)
    {
        _log("INS_LDA_AB failed.", 1);
    }
    else
    {
        _log("INS_LDA_AB succeeded.", 0);
    }
    testLDAFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDA_ABX; // Load byte from memory into Accumulator from absolute address (2 byte) + X
    mem.Data[0xFFFD] = 0x12;
    mem.Data[0xFFFE] = 0x34;
    mem.Data[0x3511] = 0x42;
    cpu.X = 0xFF;
    execute(CYC_LDA_ABX_PC, &mem, &cpu);
    if (!cpu.A)
    {
        _log("INS_LDA_ABX failed.", 1);
    }
    else
    {
        _log("INS_LDA_ABX succeeded.", 0);
    }
    testLDAFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDA_ABY; // Load byte from memory into Accumulator from absolute address (2 byte) + Y
    mem.Data[0xFFFD] = 0x12;
    mem.Data[0xFFFE] = 0x34;
    mem.Data[0x3511] = 0x42;
    cpu.Y = 0xFF;
    execute(CYC_LDA_ABY_PC, &mem, &cpu);
    if (!cpu.A)
    {
        _log("INS_LDA_ABY failed.", 1);
    }
    else
    {
        _log("INS_LDA_ABY succeeded.", 0);
    }
    testLDAFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDA_INDX;
    mem.Data[0xFFFD] = 0x01;
    mem.Data[0x0002] = 0x04;
    mem.Data[0x0003] = 0x04;
    mem.Data[0x0404] = 0x42;
    cpu.X = 0x01;
    execute(CYC_LDA_INDX, &mem, &cpu);
    if (!cpu.A)
    {
        _log("INS_LDA_INDX failed.", 1);
    }
    else
    {
        _log("INS_LDA_INDX succeeded.", 0);
    }
    testLDAFlags(&cpu, &cpucopy, &mem);

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDA_INDY; 
    mem.Data[0xFFFD] = 0x01;
    mem.Data[0x0001] = 0x04;
    mem.Data[0x0002] = 0x04;
    mem.Data[0x0503] = 0x42;
    cpu.Y = 0xFF;
    execute(CYC_LDA_INDY_PC, &mem, &cpu);
    if (!cpu.A)
    {
        _log("INS_LDA_INDY failed.", 1);
    }
    else
    {
        _log("INS_LDA_INDY succeeded.", 0);
    }
    testLDAFlags(&cpu, &cpucopy, &mem);

    printf("Test LDA finished with %d error(s).\n\n\n", errors);
}