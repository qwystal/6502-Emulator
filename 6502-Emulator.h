#pragma once
#include <stdlib.h>
#include <stdio.h>

#define EXEC_START 0xFFFC
#define MAX_MEM 1024*64

// Cycles req. Load Accumulator (LDA)
#define CYC_LDA_IM 2 // Cycles required for Load Accumulator Immediate
#define CYC_LDA_ZP 3
#define CYC_LDA_ZPX 4
#define CYC_LDA_AB 4
#define CYC_LDA_ABX 4
#define CYC_LDA_ABX_PC 5 // PC if page is crossed. means if the X register + Byte = >255, you have to increase a second byte.
#define CYC_LDA_ABY 4
#define CYC_LDA_ABY_PC 5
#define CYC_LDA_INDX 6
#define CYC_LDA_INDY 5
#define CYC_LDA_INDY_PC 6

// Cycles req. Load X (LDX)
#define CYC_LDX_IM 2
#define CYC_LDX_ZP 3
#define CYC_LDX_ZPY 4
#define CYC_LDX_AB 4
#define CYC_LDX_ABY 4
#define CYC_LDX_ABY_PC 5

// Cycles req. Load Y (LDY)
#define CYC_LDY_IM 2
#define CYC_LDY_ZP 3
#define CYC_LDY_ZPX 4
#define CYC_LDY_AB 4
#define CYC_LDY_ABX 4
#define CYC_LDY_ABX_PC 5

// Cycles req. Store Accumulator (STA)
#define CYC_STA_ZP 3
#define CYC_STA_ZPX 4
#define CYC_STA_AB 4
#define CYC_STA_ABX 5
#define CYC_STA_ABY 5
#define CYC_STA_INDX 6
#define CYC_STA_INDY 6

// Cycles req. Store X Register (STX)
#define CYC_STX_ZP 3
#define CYC_STX_ZPY 4
#define CYC_STX_AB 4

// Cycles req. Store Y Register (STY)
#define CYC_STY_ZP 3
#define CYC_STY_ZPX 4
#define CYC_STY_AB 4

// Load Accumulator (LDA)
#define INS_LDA_IM 0xA9 // Load Accumulator Immediate
#define INS_LDA_ZP 0xA5 // Load Accumulator Zero Page
#define INS_LDA_ZPX 0xB5 // Load Accumulator Zero Page,X
#define INS_LDA_AB 0xAD // Load Accumulator Absolute
#define INS_LDA_ABX 0xBD // Load Accumulator Absolute,X
#define INS_LDA_ABY 0xB9 // Load Accumulator Absolute,Y
#define INS_LDA_INDX 0xA1 // Load Accumulator Indirect,X
#define INS_LDA_INDY 0xB1 // Load Accumulator Indirect,Y

// Load X (LDX)
#define INS_LDX_IM 0xA2
#define INS_LDX_ZP 0xA6
#define INS_LDX_ZPY 0xB6
#define INS_LDX_AB 0xAE
#define INS_LDX_ABY 0xBE

// Load Y (LDY)
#define INS_LDY_IM 0xA0
#define INS_LDY_ZP 0xA4
#define INS_LDY_ZPX 0xB4
#define INS_LDY_AB 0xAC
#define INS_LDY_ABX 0xBC

// Store Accumulator (STA)
#define INS_STA_ZP 0x85
#define INS_STA_ZPX 0x95
#define INS_STA_AB 0x8D
#define INS_STA_ABX 0x9D
#define INS_STA_ABY 0x99
#define INS_STA_INDX 0x81
#define INS_STA_INDY 0x91

// Store X Register
#define INS_STX_ZP 0x86
#define INS_STX_ZPY 0x96
#define INS_STX_AB 0x8E

// Store Y Register
#define INS_STY_ZP 0x84
#define INS_STY_ZPX 0x94
#define INS_STY_AB 0x8C

// !not an exact emulation!
// http://archive.6502.org/datasheets/wdc_w65c02s_mar_2000.pdf

typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int u32;
typedef signed int s32;

int success;

void _success(){
    success = 1;
}

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
    for (size_t i = 0; i < MAX_MEM; i++)
    {
        mem->Data[i] = 0;
    }
}

void ResetCPU(CPU *cpu)
{
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
}

void start(CPU *cpu, Memory *mem)
{
    ResetCPU(cpu);
    initializeMemory(mem);
}

Byte fetchByte(s32 *cycles, const Memory *mem, CPU *cpu)
{
    Byte data = mem->Data[cpu->PC];
    cpu->PC++;
    (*cycles)--;
    return data;
}

Byte readByte(s32 *cycles, const Memory *mem, Word address)
{
    Byte data = mem->Data[address];
    (*cycles)--;
    return data;
}

Word fetchWord(s32 *cycles, const Memory *mem, CPU *cpu)
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

Word readWord(s32 *cycles, const Memory *mem, Word address)
{
    Byte LowByte = readByte(cycles, mem, address);
    Byte HighByte = readByte(cycles, mem, address + 0x01);
    return LowByte | (HighByte << 8);
}

void writeByte(s32 *cycles, Memory *mem, Word address, Byte data)
{
    mem->Data[address] = data;
    (*cycles)--;
}

void LDASetFlag(CPU *cpu) // Load Accumulator Status Flags
{
    cpu->Z = (cpu->A == 0);
    cpu->N = (getBitOfByte(cpu->A, 7) == 1);
}

void LDXSetFlag(CPU *cpu) // Load X Register Status Flags
{
    cpu->Z = (cpu->X == 0);
    cpu->N = (getBitOfByte(cpu->X, 7) == 1);
}

void LDYSetFlag(CPU *cpu) // Load Y Register Status Flags
{
    cpu->Z = (cpu->Y == 0);
    cpu->N = (getBitOfByte(cpu->Y, 7) == 1);
}

Byte getZeroPageAddress(s32 *cycles, const Memory *mem, CPU *cpu)
{
    return fetchByte(cycles, mem, cpu);
}

Byte getZeroPageAddressX(s32 *cycles, const Memory *mem, CPU *cpu)
{
    Byte ZeroPageAddress = fetchByte(cycles, mem, cpu);
    (*cycles)--;
    return ZeroPageAddress + cpu->X;
}

Byte getZeroPageAddressY(s32 *cycles, const Memory *mem, CPU *cpu)
{
    Byte ZeroPageAddress = fetchByte(cycles, mem, cpu);
    (*cycles)--;
    return ZeroPageAddress + cpu->Y;
}

Word getAbsoluteAddress(s32 *cycles, const Memory *mem, CPU *cpu)
{
    return fetchWord(cycles, mem, cpu);
}

Word getAbsoluteAddressX(s32 *cycles, const Memory *mem, CPU *cpu)
{
    Word AbsoluteAddress = fetchWord(cycles, mem, cpu);
    Word AbsoluteAddressX = AbsoluteAddress + cpu->X;
    (*cycles) -= (AbsoluteAddressX - AbsoluteAddress >= 0xFF) ? 1 : 0;
    return AbsoluteAddressX;
}

Word getAbsoluteAddressY(s32 *cycles, const Memory *mem, CPU *cpu)
{
    Word AbsoluteAddress = fetchWord(cycles, mem, cpu);
    Word AbsoluteAddressY = AbsoluteAddress + cpu->Y;
    (*cycles) -= (AbsoluteAddressY - AbsoluteAddress >= 0xFF) ? 1 : 0;
    return AbsoluteAddressY;
}

Word getIndirectX(s32 *cycles, const Memory *mem, CPU *cpu)
{
    Byte ZeroPageAddress = getZeroPageAddressX(cycles, mem, cpu);
    (*cycles)--;
    Word EffectiveAddress = readWord(cycles, mem, ZeroPageAddress);
    return EffectiveAddress;
}

Word getIndirectY(s32 *cycles, const Memory *mem, CPU *cpu)
{
    Byte ZeroPageAddress = fetchByte(cycles, mem, cpu);
    Word EffectiveAddress = readWord(cycles, mem, ZeroPageAddress);
    Word EffectiveAddressY = EffectiveAddress + cpu->Y;
    (*cycles) -= (EffectiveAddressY - EffectiveAddress >= 0xFF) ? 1 : 0;
    return EffectiveAddressY;
}

s32 LDA(s32 cycles, const Memory *mem, CPU *cpu, Byte ins)
{
    Byte ZeroPageAddress;
    Word AbsoluteAddress;

    switch (ins)
        {
        case INS_LDA_IM:
            cpu->A = fetchByte(&cycles, mem, cpu);
            LDASetFlag(cpu);
            _success();
            break;

        case INS_LDA_ZP:
            ZeroPageAddress = getZeroPageAddress(&cycles, mem, cpu);
            cpu->A = readByte(&cycles, mem, ZeroPageAddress);
            LDASetFlag(cpu);
            _success();
            break;

        case INS_LDA_ZPX: /* IMPORTANT Address can overflow! But How???*/
            ZeroPageAddress = getZeroPageAddressX(&cycles, mem, cpu);
            cpu->A = readByte(&cycles, mem, ZeroPageAddress);
            LDASetFlag(cpu);
            _success();
            break;

        case INS_LDA_AB:
            AbsoluteAddress = getAbsoluteAddress(&cycles, mem, cpu);
            cpu->A = readByte(&cycles, mem, AbsoluteAddress);
            LDASetFlag(cpu);
            _success();
            break;

        case INS_LDA_ABX:
            AbsoluteAddress = getAbsoluteAddressX(&cycles, mem, cpu);
            cpu->A = readByte(&cycles, mem, AbsoluteAddress);
            LDASetFlag(cpu);
            _success();
            break;

        case INS_LDA_ABY:
            AbsoluteAddress = getAbsoluteAddressY(&cycles, mem, cpu);
            cpu->A = readByte(&cycles, mem, AbsoluteAddress);
            LDASetFlag(cpu);
            _success();
            break;

        case INS_LDA_INDX:
            AbsoluteAddress = getIndirectX(&cycles, mem, cpu);
            cpu->A = readByte(&cycles, mem, AbsoluteAddress);
            LDASetFlag(cpu);
            _success();
            break;

        case INS_LDA_INDY:
            AbsoluteAddress = getIndirectY(&cycles, mem, cpu);
            cpu->A = readByte(&cycles, mem, AbsoluteAddress);
            _success();
            break;
        }

    return cycles;
}

s32 LDX(s32 cycles, const Memory *mem, CPU *cpu, Byte ins)
{
    Byte ZeroPageAddress;
    Word AbsoluteAddress;
    switch (ins)
        {
        case INS_LDX_IM:
            cpu->X = fetchByte(&cycles, mem, cpu);
            LDXSetFlag(cpu);
            _success();
            break;

        case INS_LDX_ZP:
            ZeroPageAddress = getZeroPageAddress(&cycles, mem, cpu);
            cpu->X = readByte(&cycles, mem, ZeroPageAddress);
            LDXSetFlag(cpu);
            _success();
            break;

        case INS_LDX_ZPY: /* IMPORTANT Address can overflow! But How???*/
            ZeroPageAddress = getZeroPageAddressY(&cycles, mem, cpu);
            cpu->X = readByte(&cycles, mem, ZeroPageAddress);
            LDXSetFlag(cpu);
            _success();
            break;

        case INS_LDX_AB:
            AbsoluteAddress = getAbsoluteAddress(&cycles, mem, cpu);
            cpu->X = readByte(&cycles, mem, AbsoluteAddress);
            LDXSetFlag(cpu);
            _success();
            break;

        case INS_LDX_ABY:
            AbsoluteAddress = getAbsoluteAddressY(&cycles, mem, cpu);
            cpu->X = readByte(&cycles, mem, AbsoluteAddress);
            LDXSetFlag(cpu);
            _success();
            break;
        }
        
    return cycles;
}

s32 LDY(s32 cycles, const Memory *mem, CPU *cpu, Byte ins)
{
    Byte ZeroPageAddress;
    Word AbsoluteAddress;
    switch (ins)
        {
        case INS_LDY_IM:
            cpu->Y = fetchByte(&cycles, mem, cpu);
            LDYSetFlag(cpu);
            _success();
            break;

        case INS_LDY_ZP:
            ZeroPageAddress = getZeroPageAddress(&cycles, mem ,cpu);
            cpu->Y = readByte(&cycles, mem, ZeroPageAddress);
            LDYSetFlag(cpu);
            _success();
            break;

        case INS_LDY_ZPX: /* IMPORTANT Address can overflow! But How???*/
            ZeroPageAddress = getZeroPageAddressX(&cycles, mem, cpu);
            cpu->Y = readByte(&cycles, mem, ZeroPageAddress);
            LDYSetFlag(cpu);
            _success();
            break;

        case INS_LDY_AB:
            AbsoluteAddress = getAbsoluteAddress(&cycles, mem, cpu);
            cpu->Y = readByte(&cycles, mem, AbsoluteAddress);
            LDXSetFlag(cpu);
            _success();
            break;

        case INS_LDY_ABX:
            AbsoluteAddress = getAbsoluteAddressX(&cycles, mem, cpu);
            cpu->Y = readByte(&cycles, mem, AbsoluteAddress);
            LDXSetFlag(cpu);
            _success();
            break;
        }
        
    return cycles;
}

s32 STA(s32 cycles, Memory *mem, CPU *cpu, Byte ins)
{
    Byte ZeroPageAddress;
    Word AbsoluteAddress;
    switch (ins)
    {
        case INS_STA_ZP:
            ZeroPageAddress = getZeroPageAddress(&cycles, mem, cpu);
            writeByte(&cycles, mem, ZeroPageAddress, cpu->A);
            _success();
            break;

        case INS_STA_ZPX:
            ZeroPageAddress = getZeroPageAddressX(&cycles, mem, cpu);
            writeByte(&cycles, mem, ZeroPageAddress, cpu->A);
            _success();
            break;

        case INS_STA_AB:
            AbsoluteAddress = getAbsoluteAddress(&cycles, mem, cpu);
            writeByte(&cycles, mem, AbsoluteAddress, cpu->A);
            _success();
            break;

        case INS_STA_ABX:
            AbsoluteAddress = getAbsoluteAddressX(&cycles, mem, cpu);
            writeByte(&cycles, mem, AbsoluteAddress, cpu->A);
            _success();
            break;

        case INS_STA_ABY:
            AbsoluteAddress = getAbsoluteAddressY(&cycles, mem, cpu);
            writeByte(&cycles, mem, AbsoluteAddress, cpu->A);
            _success();
            break;

        case INS_STA_INDX:
            AbsoluteAddress = getIndirectX(&cycles, mem, cpu);
            writeByte(&cycles, mem, AbsoluteAddress, cpu->A);
            _success();
            break;

        case INS_STA_INDY:
            AbsoluteAddress = getIndirectY(&cycles, mem, cpu);
            writeByte(&cycles, mem, AbsoluteAddress, cpu->A);
            _success();
            break;
    }
    return cycles;
}

s32 STX(s32 cycles, Memory *mem, CPU *cpu, Byte ins)
{
    Byte ZeroPageAddress;
    Word AbsoluteAddress;
    switch (ins)
    {
        case INS_STX_ZP:
            ZeroPageAddress = getZeroPageAddress(&cycles, mem, cpu);
            writeByte(&cycles, mem, ZeroPageAddress, cpu->X);
            _success();
            break;

        case INS_STX_ZPY:
            ZeroPageAddress = getZeroPageAddressY(&cycles, mem, cpu);
            writeByte(&cycles, mem, ZeroPageAddress, cpu->X);
            _success();
            break;

        case INS_STX_AB:
            AbsoluteAddress = getAbsoluteAddress(&cycles, mem, cpu);
            writeByte(&cycles, mem, AbsoluteAddress, cpu->X);
            _success();
            break;
    }

    return cycles;
}

s32 STY(s32 cycles, Memory *mem, CPU *cpu, Byte ins)
{
    Byte ZeroPageAddress;
    Word AbsoluteAddress;
    switch (ins)
    {
        case INS_STY_ZP:
            ZeroPageAddress = getZeroPageAddress(&cycles, mem, cpu);
            writeByte(&cycles, mem, ZeroPageAddress, cpu->Y);
            _success();
            break;

        case INS_STY_ZPX:
            ZeroPageAddress = getZeroPageAddressX(&cycles, mem, cpu);
            writeByte(&cycles, mem, ZeroPageAddress, cpu->Y);
            _success();
            break;

        case INS_STY_AB:
            AbsoluteAddress = getAbsoluteAddress(&cycles, mem, cpu);
            writeByte(&cycles, mem, AbsoluteAddress, cpu->Y);
            _success();
            break;
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
        cycles = LDX(cycles, mem, cpu, ins);
        cycles = LDY(cycles, mem, cpu, ins);
        cycles = STA(cycles, mem, cpu, ins);
        cycles = STX(cycles, mem, cpu, ins);
        cycles = STY(cycles, mem, cpu, ins);

        success ? 0 : printf("Unknown instruction %c.\n", ins);
    }

    cycles < 0 ? printf("Execution failed. Too few cycles.\n") : printf("Execution finished.\n");
}