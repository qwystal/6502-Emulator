#pragma once
#include <stdlib.h>
#include <stdio.h>

#define EXEC_START 0xFFFC
#define MAX_MEM 1024*64

#define CYC_IM 2
#define CYC_ZP 3
#define CYC_ZPX 4
#define CYC_AB 4
#define CYC_ABX 4
#define CYC_ABX_PC 5 // PC if page is crossed. means if the X register + Byte = >255, you have to increase a second byte.
#define CYC_ABY 4
#define CYC_ABY_PC 5
#define CYC_INDX 6
#define CYC_INDY 5
#define CYC_INDY_PC 6

#define INS_LDA_IM 0xA9 // Load Accumulator Immediate
#define INS_LDA_ZP 0xA5 // Load Accumulator Zero Page
#define INS_LDA_ZPX 0xB5 // Load Accumulator Zero Page,X
#define INS_LDA_AB 0xAD // Load Accumulator Absolute
#define INS_LDA_ABX 0xBD // Load Accumulator Absolute,X
#define INS_LDA_ABY 0xB9 // Load Accumulator Absolute,Y
#define INS_LDA_INDX 0xA1 // Load Accumulator Indirect,X
#define INS_LDA_INDY 0xB1 // Load Accumulator Indirect,Y

// !not an exact emulation!
// http://archive.6502.org/datasheets/wdc_w65c02s_mar_2000.pdf

typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int u32;
typedef signed int s32;

int unknownInstruction;

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

Byte fetchByte(s32 *cycles, Memory *mem, CPU *cpu)
{
    Byte data = mem->Data[cpu->PC];
    cpu->PC++;
    (*cycles)--;
    return data;
}

Byte readByte(s32 *cycles, Memory *mem, Word address)
{
    Byte data = mem->Data[address];
    (*cycles)--;
    return data;
}

Word fetchWord(s32 *cycles, Memory *mem, CPU *cpu)
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

Word readWord(s32 *cycles, Memory *mem, Word address)
{
    Byte LowByte = readByte(cycles, mem, address);
    Byte HighByte = readByte(cycles, mem, address + 0x01);
    return LowByte | (HighByte << 8);
}

void LDASetStatus(CPU *cpu, Byte value) // Load Accumulator Status Flags
{
    cpu->Z = (cpu->A == 0);
    cpu->N = (getBitOfByte(value, 7) == 1);
}

s32 LDA(s32 cycles, Memory *mem, CPU *cpu, Byte ins)
{
    Byte ZeroPageAddress;
    Word AbsoluteAddress;
    Word EffectiveAddress;
    Word _AbsoluteAddress;
    Word _EffectiveAddress;
    unknownInstruction = 0;
    switch (ins)
        {
        case INS_LDA_IM:
            cpu->A = fetchByte(&cycles, mem, cpu);
            LDASetStatus(cpu, cpu->A);
            break;

        case INS_LDA_ZP:
            ZeroPageAddress = fetchByte(&cycles, mem, cpu);
            cpu->A = readByte(&cycles, mem, ZeroPageAddress);
            LDASetStatus(cpu, cpu->A);
            break;

        case INS_LDA_ZPX: /* IMPORTANT Address can overflow! But How???*/
            ZeroPageAddress = fetchByte(&cycles, mem, cpu);
            ZeroPageAddress += cpu->X;
            cycles--;
            cpu->A = readByte(&cycles, mem, ZeroPageAddress);
            LDASetStatus(cpu, cpu->A);
            break;

        case INS_LDA_AB:
            AbsoluteAddress = fetchWord(&cycles, mem, cpu);
            cpu->A = readByte(&cycles, mem, AbsoluteAddress);
            LDASetStatus(cpu, cpu->A);
            break;

        case INS_LDA_ABX:
            _AbsoluteAddress = fetchWord(&cycles, mem, cpu);
            AbsoluteAddress = _AbsoluteAddress + cpu->X;
            cpu->A = readByte(&cycles, mem, AbsoluteAddress);
            LDASetStatus(cpu, cpu->A);
            if (AbsoluteAddress - _AbsoluteAddress >= 0xFF)
            {
                cycles--;
            }
            break;

        case INS_LDA_ABY:
            _AbsoluteAddress = fetchWord(&cycles, mem, cpu);
            AbsoluteAddress = _AbsoluteAddress + cpu->Y;
            cpu->A = readByte(&cycles, mem, AbsoluteAddress);
            LDASetStatus(cpu, cpu->A);
            if (AbsoluteAddress - _AbsoluteAddress >= 0xFF)
            {
                cycles--;
            }
            break;

        case INS_LDA_INDX:
            ZeroPageAddress = fetchByte(&cycles, mem, cpu);
            ZeroPageAddress += cpu->X;
            cycles--;
            EffectiveAddress = readWord(&cycles, mem, ZeroPageAddress);
            cpu->A = readByte(&cycles, mem, EffectiveAddress);
            LDASetStatus(cpu, cpu->A);
            break;

        case INS_LDA_INDY:
            ZeroPageAddress = fetchByte(&cycles, mem, cpu);
            EffectiveAddress = readWord(&cycles, mem, ZeroPageAddress);
            _EffectiveAddress = EffectiveAddress + cpu->Y;
            cpu->A = readByte(&cycles, mem, _EffectiveAddress);
            if (_EffectiveAddress - EffectiveAddress >= 0xFF)
            {
                cycles--;
            }
            break;
        
        default:
            unknownInstruction = 1;
        }
    return cycles;
}

void execute(s32 cycles, Memory *mem, CPU *cpu)
{
    printf("Beginning execution.\n");
    const s32 _cycles = cycles;
    Byte ins;
    while (cycles > 0)
    {
        ins = fetchByte(&cycles, mem, cpu);

        cycles = LDA(cycles, mem, cpu, ins);

        if (unknownInstruction == 1)
        {
            printf("Unknown instruction %c.\n", ins);
        }
    }

    if (cycles < 0)
    {
        printf("Execution failed. Too few cycles.\n");
    }
    else
    {
        printf("Execution finished.\n");
    }
}