#pragma once
#include "6502-Emulator.h"

int errors;

void _log(char *text, int errorlevel){
    if (errorlevel == 0)
    {
        printf("\033[32m[+]\033[0m %s\n", text);
    }
    if (errorlevel == 1)
    {
        printf("\033[31m[-]\033[0m %s\n", text);
        errors += 1;
    }
}

void testLDA()
{
    CPU cpu;
    Memory mem;

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDA_IM; // Load byte from memory into Accumulator immediatly
    mem.Data[0xFFFD] = 0x01;
    execute(CYC_IM, &mem, &cpu);
    if (!cpu.A)
    {
        _log("INS_LDA_IM failed.", 1);
    }
    else
    {
        _log("INS_LDA_IM succeeded.", 0);
    }

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDA_ZP; // Load byte from memory into Accumulator from pointer
    mem.Data[0xFFFD] = 0x42;
    mem.Data[0x0042] = 0x01;
    execute(CYC_ZP, &mem, &cpu);
    if (!cpu.A)
    {
        _log("INS_LDA_ZP failed.", 1);
    }
    else
    {
        _log("INS_LDA_ZP succeeded.", 0);
    }

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDA_ZPX; // Load byte from memory into Accumulator from pointer + X
    cpu.X = 0x01;
    mem.Data[0xFFFD] = 0x42;
    mem.Data[0x0043] = 0x01;
    execute(CYC_ZPX, &mem, &cpu);
    if (!cpu.A)
    {
        _log("INS_LDA_ZPX failed.", 1);
    }
    else
    {
        _log("INS_LDA_ZPX succeeded.", 0);
    }

    start(&cpu, &mem);
    mem.Data[EXEC_START] = INS_LDA_AB; // Load byte from memory into Accumulator from absolute address (2 byte)
    mem.Data[0xFFFD] = 0x12;
    mem.Data[0xFFFE] = 0x34;
    mem.Data[0x3412] = 0x42;
    execute(CYC_AB, &mem, &cpu);
    if (!cpu.A)
    {
        _log("INS_LDA_AB failed.", 1);
    }
    else
    {
        _log("INS_LDA_AB succeeded.", 0);
    }
    printf("Test LDA finished with %d errors.", errors);
}