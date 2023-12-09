#include "includes.h"

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
