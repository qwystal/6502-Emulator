#include <stdlib.h>
#include <stdio.h>

#define MAX_MEM 1024*64
#define INS_LDA_IM 0xA9 // Load Address Immediate
#define INS_LDA_ZP 0xA5 // Load Address Zero Page
#define INS_LDA_ZPX 0xB5 // Load Address Zero Page,X

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

void InitialiseMemory(Memory *mem)
{
    printf("Initialising memory.\n");
    for (size_t i = 0; i < MAX_MEM; i++)
    {
        mem->Data[i] = 0;
    }
    printf("Memory initialised.\n");
}

void ResetCPU(CPU *cpu)
{
    printf("Resetting CPU...");
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
    InitialiseMemory(mem);
    printf("CPU started.\n");
}

Byte fetchByte(u32 *cycles, Memory *mem, CPU *cpu)
{
    Byte data = mem->Data[cpu->PC];
    cpu->PC++;
    (*cycles)--;
    return data;
}

Byte readByte(u32 *cycles, Memory *mem, Byte address)
{
    Byte data = mem->Data[address];
    (*cycles)--;
    return data;
}

void LDASetStatus(CPU *cpu, Byte value)
{
    cpu->Z = (cpu->A == 0);
    cpu->N = (getBitOfByte(value, 7) == 1);
}

void execute(u32 cycles, Memory *mem, CPU *cpu)
{
    printf("Beginning execution.\n");
    while (cycles > 0)
    {
        printf("Beginning cycle %d.\n", cycles);
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

        case INS_LDA_ZPX: /* IMPORTANT Address can overflow! */
            Byte ZeroPageAddress = fetchByte(&cycles, mem, cpu);
            ZeroPageAddress += cpu->X;
            cycles--;
            cpu->A = readByte(&cycles, mem, ZeroPageAddress);
            LDASetStatus(cpu, cpu->A);
            break;
        
        default:
            printf("Unknown instruction %c.\n", ins);
            break;
        }
    }
    
}

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