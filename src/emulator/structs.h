#ifndef STRUCTS_H
#define STRUCTS_H

#include "typedefs.h"

typedef struct Memory
{
    Byte Data[MAX_MEM];
} Memory;


typedef struct CPU
{
    Word PC; // Program Counter
    Word SP; // Stack Pointer, limited from 0x0100 to 0x01FF in memory
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
} CPU;

#endif