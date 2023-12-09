#include "includes.h"

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