#include "includes.h"

int success;

void _success()
{
    success = 1;
}

void execute(s32 cycles, Memory *mem, CPU *cpu)
{
    printf("Beginning execution.\n");
    const s32 _cycles = cycles;
    Byte ins; // Instruction
    while (cycles > 0)
    {
        ins = fetchByte(&cycles, mem, cpu);

        cycles = LDA(cycles, mem, cpu, ins);
        cycles = LDX(cycles, mem, cpu, ins);
        cycles = LDY(cycles, mem, cpu, ins);
        cycles = STA(cycles, mem, cpu, ins);
        cycles = STX(cycles, mem, cpu, ins);
        cycles = STY(cycles, mem, cpu, ins);
        
        if (!success)
        {
            printf("Unknown instruction %c.\n", ins);
        }
    }

    if (cycles < 0) 
    {
        printf("Execution failed. Too few cycles.\n");
    } 
    else if (cycles > 0) 
    {
        printf("Execution failed. Too many cycles.\n");
    } 
    else if (cycles == 0)
    {
        printf("Execution finished.\n");
    }
}