#include "includes.h"

// !not an exact emulation!
// http://archive.6502.org/datasheets/wdc_w65c02s_mar_2000.pdf

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

void initializeMemory(Memory *mem)
{
    for (size_t i = 0; i < MAX_MEM; i++)
    {
        mem->Data[i] = 0;
    }
}

void resetCPU(CPU *cpu)
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
    resetCPU(cpu);
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

Word getAbsoluteAddressX_5(s32 *cycles, const Memory *mem, CPU *cpu)
{
    Word AbsoluteAddress = fetchWord(cycles, mem, cpu);
    Word AbsoluteAddressX = AbsoluteAddress + cpu->X;
    (*cycles)--;
    return AbsoluteAddressX;
}

Word getAbsoluteAddressY(s32 *cycles, const Memory *mem, CPU *cpu)
{
    Word AbsoluteAddress = fetchWord(cycles, mem, cpu);
    Word AbsoluteAddressY = AbsoluteAddress + cpu->Y;
    (*cycles) -= (AbsoluteAddressY - AbsoluteAddress >= 0xFF) ? 1 : 0;
    return AbsoluteAddressY;
}

Word getAbsoluteAddressY_5(s32 *cycles, const Memory *mem, CPU *cpu)
{
    Word AbsoluteAddress = fetchWord(cycles, mem, cpu);
    Word AbsoluteAddressY = AbsoluteAddress + cpu->Y;
    (*cycles)--;
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

Word getIndirectY_6(s32 *cycles, const Memory *mem, CPU *cpu)
{
    Byte ZeroPageAddress = fetchByte(cycles, mem, cpu);
    Word EffectiveAddress = readWord(cycles, mem, ZeroPageAddress);
    Word EffectiveAddressY = EffectiveAddress + cpu->Y;
    (*cycles)--;
    return EffectiveAddressY;
}