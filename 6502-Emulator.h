#pragma once
#include <stdlib.h>
#include <stdio.h>

#define MAX_MEM 1024*64
#define INS_LDA_IM 0xA9 // Load Accumulator Immediate
#define INS_LDA_ZP 0xA5 // Load Accumulator Zero Page
#define INS_LDA_ZPX 0xB5 // Load Accumulator Zero Page,X
#define INS_LDA_AB 0xAD // Load Accumulator Absolute
#define INS_LDA_ABX 0xBD // Load Accumulator Absolute,X

// !not an exact emulation!
// http://archive.6502.org/datasheets/wdc_w65c02s_mar_2000.pdf

typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int u32;

Byte setBitOfByte(Byte value, int position)
{
    Byte mask = (1 << position);
    return value | mask;
}

Byte getBitOfByte(Byte value, int position)
{
    Byte mask = (1 << position);
    return (value & mask) ? 1 : 0;
}

Word setBitOfWord(Word value, int position)
{
    Word mask = (1 << position);
    return value | mask;
}

Word getBitOfWord(Word value, int position)
{
    Word mask = (1 << position);
    return (value & mask) ? 1 : 0;
}

typedef struct Memory
{
    Byte Data[MAX_MEM];
}Memory;


typedef struct CPU
{
    Word PC; // Program Counter
    Word SP; // Stack Pointer
    Byte A; // Accumulator (Register)
    Byte X, Y; // Index Registers
    
    struct { // Processor Status Registers
        Byte N : 1; // Negative flag
        Byte V : 1; // Overflow flag
        Byte B : 1; // Break flag
        Byte D : 1; // Decimal mode flag
        Byte I : 1; // Interrupt disable flag
        Byte Z : 1; // Zero flag
        Byte C : 1; // Carry flag
    };
}CPU;

void initializeMemory(Memory *mem)
{
    printf("Initializing memory...\n");
    for (size_t i = 0; i < MAX_MEM; i++)
    {
        mem->Data[i] = 0;
    }
    printf("Memory initialized.\n");
}

void ResetCPU(CPU *cpu)
{
    printf("Resetting CPU...\n");
    cpu->PC = 0xFFFC;
    cpu->SP = 0x0100;
    cpu->C = 0;
    cpu->Z = 0;
    cpu->I = 0;
    cpu->D = 0;
    cpu->B = 0;
    cpu->V = 0;
    cpu->N = 0;
    
    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;
    printf("CPU reset.\n");
}

void start(CPU *cpu, Memory *mem)
{
    printf("Starting CPU.\n");
    ResetCPU(cpu);
    initializeMemory(mem);
    printf("CPU started.\n");
}

Byte fetchByte(u32 *cycles, Memory *mem, CPU *cpu)
{
    Byte data = mem->Data[cpu->PC];
    cpu->PC++;
    (*cycles)--;
    return data;
}

Byte readByte(u32 *cycles, Memory *mem, Word address)
{
    Byte data = mem->Data[address];
    (*cycles)--;
    return data;
}

Word fetchWord(u32 *cycles, Memory *mem, CPU *cpu)
{
    Word data = mem->Data[cpu->PC];
    cpu->PC++;

    data |= (mem->Data[cpu->PC] << 8);
    cpu->PC++;
    (*cycles) -= 2;

    /*
    If you want a Big Endian system, you have to swap bytes here.
    if (BIG_ENDIAN)
        swapBytesInWord(data)
    */

    return data;
}

void LDASetStatus(CPU *cpu, Byte value) // Load Accumulator Status Flags
{
    cpu->Z = (cpu->A == 0);
    cpu->N = (getBitOfByte(value, 7) == 1);
}

void execute(u32 cycles, Memory *mem, CPU *cpu)
{
    printf("Beginning execution.\n");
    while (cycles > 0)
    {
        Byte ins = fetchByte(&cycles, mem, cpu);
        switch (ins)
        {
        case INS_LDA_IM:
            Byte value = fetchByte(&cycles, mem, cpu);
            cpu->A = value;
            LDASetStatus(cpu, value);
            break;

        case INS_LDA_ZP:
            Byte ZeroPageAddress = fetchByte(&cycles, mem, cpu);
            cpu->A = readByte(&cycles, mem, ZeroPageAddress);
            LDASetStatus(cpu, cpu->A);
            break;

        case INS_LDA_ZPX: /* IMPORTANT Address can overflow! What does that mean???*/
            Byte ZeroPageAddressX = fetchByte(&cycles, mem, cpu);
            ZeroPageAddressX += cpu->X;
            cycles--;
            cpu->A = readByte(&cycles, mem, ZeroPageAddressX);
            LDASetStatus(cpu, cpu->A);
            break;

        case INS_LDA_AB:
            Word AbsoluteAddress = fetchWord(&cycles, mem, cpu);
            cpu->A = readByte(&cycles, mem, AbsoluteAddress);
            LDASetStatus(cpu, cpu->A);
            break;
        
        default:
            printf("Unknown instruction %c.\n", ins);
            break;
        }
    }
    printf("Execution finished.\n");
}