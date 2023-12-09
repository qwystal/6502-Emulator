#include "includes.h"

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