#include "includes.h"

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
            AbsoluteAddress = getAbsoluteAddressX_5(&cycles, mem, cpu);
            writeByte(&cycles, mem, AbsoluteAddress, cpu->A);
            _success();
            break;

        case INS_STA_ABY:
            AbsoluteAddress = getAbsoluteAddressY_5(&cycles, mem, cpu);
            writeByte(&cycles, mem, AbsoluteAddress, cpu->A);
            _success();
            break;

        case INS_STA_INDX:
            AbsoluteAddress = getIndirectX(&cycles, mem, cpu);
            writeByte(&cycles, mem, AbsoluteAddress, cpu->A);
            _success();
            break;

        case INS_STA_INDY:
            AbsoluteAddress = getIndirectY_6(&cycles, mem, cpu);
            writeByte(&cycles, mem, AbsoluteAddress, cpu->A);
            _success();
            break;
    }
    return cycles;
}