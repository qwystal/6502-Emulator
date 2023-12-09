#include "includes.h"

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