#include "includes.h"

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
