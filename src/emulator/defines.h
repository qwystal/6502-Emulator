#ifndef DEFINES_H
#define DEFINES_H

// Execution start address
#define EXEC_START 0xFFFC
// Memory size
#define MAX_MEM 1024*64

// Cycles required for Load Accumulator Immediate (LDA)
#define CYC_LDA_IM 2
// Cycles required for Load Accumulator Zero Page (LDA)
#define CYC_LDA_ZP 3
// Cycles required for Load Accumulator Zero Page,X (LDA)
#define CYC_LDA_ZPX 4
// Cycles required for Load Accumulator Absolute (LDA)
#define CYC_LDA_AB 4
// Cycles required for Load Accumulator Absolute,X (LDA)
#define CYC_LDA_ABX 4
// Cycles required for Load Accumulator Absolute,X (LDA) + 1 if page crossed
#define CYC_LDA_ABX_PC 5
// Cycles required for Load Accumulator Absolute,Y (LDA)
#define CYC_LDA_ABY 4
// Cycles required for Load Accumulator Absolute,Y (LDA) + 1 if page crossed
#define CYC_LDA_ABY_PC 5
// Cycles required for Load Accumulator Indirect,X (LDA)
#define CYC_LDA_INDX 6
// Cycles required for Load Accumulator Indirect,Y (LDA)
#define CYC_LDA_INDY 5
// Cycles required for Load Accumulator Indirect,Y (LDA) + 1 if page crossed
#define CYC_LDA_INDY_PC 6

// Cycles required for Load X Immediate (LDX)
#define CYC_LDX_IM 2
// Cycles required for Load X Zero Page (LDX)
#define CYC_LDX_ZP 3
// Cycles required for Load X Zero Page,Y (LDX)
#define CYC_LDX_ZPY 4
// Cycles required for Load X Absolute (LDX)
#define CYC_LDX_AB 4
// Cycles required for Load X Absolute,Y (LDX)
#define CYC_LDX_ABY 4
// Cycles required for Load X Absolute,Y (LDX) + 1 if page crossed
#define CYC_LDX_ABY_PC 5

// Cycles required for Load Y Immediate (LDY)
#define CYC_LDY_IM 2
// Cycles required for Load Y Zero Page (LDY)
#define CYC_LDY_ZP 3
// Cycles required for Load Y Zero Page,X (LDY)
#define CYC_LDY_ZPX 4
// Cycles required for Load Y Absolute (LDY)
#define CYC_LDY_AB 4
// Cycles required for Load Y Absolute,X (LDY)
#define CYC_LDY_ABX 4
// Cycles required for Load Y Absolute,X (LDY) + 1 if page crossed
#define CYC_LDY_ABX_PC 5

// Cycles required for Store Accumulator Immediate (STA)
#define CYC_STA_ZP 3

// Cycles required for Store Accumulator Zero Page (STA)
#define CYC_STA_ZPX 4
// Cycles required for Store Accumulator Absolute (STA)
#define CYC_STA_AB 4
// Cycles required for Store Accumulator Absolute,X (STA)
#define CYC_STA_ABX 5
// Cycles required for Store Accumulator Absolute,Y (STA)
#define CYC_STA_ABY 5
// Cycles required for Store Accumulator Indirect,X (STA)
#define CYC_STA_INDX 6
// Cycles required for Store Accumulator Indirect,Y (STA)
#define CYC_STA_INDY 6

// Cycles required for Store X Register Zero Page (STX)
#define CYC_STX_ZP 3
// Cycles required for Store X Register Zero Page,Y (STX)
#define CYC_STX_ZPY 4
// Cycles required for Store X Register Absolute (STX)
#define CYC_STX_AB 4

// Cycles required for Store Y Register Zero Page (STY)
#define CYC_STY_ZP 3
// Cycles required for Store Y Register Zero Page,X (STY)
#define CYC_STY_ZPX 4
// Cycles required for Store Y Register Absolute (STY)
#define CYC_STY_AB 4

// Opcode for Load Accumulator Immediate (LDA)
#define INS_LDA_IM 0xA9
// Opcode for Load Accumulator Zero Page (LDA)
#define INS_LDA_ZP 0xA5
// Opcode for Load Accumulator Zero Page,X (LDA)
#define INS_LDA_ZPX 0xB5
// Opcode for Load Accumulator Absolute (LDA)
#define INS_LDA_AB 0xAD
// Opcode for Load Accumulator Absolute,X (LDA)
#define INS_LDA_ABX 0xBD
// Opcode for Load Accumulator Absolute,X (LDA) + 1 if page crossed
#define INS_LDA_ABY 0xB9
// Opcode for Load Accumulator Indirect,X (LDA)
#define INS_LDA_INDX 0xA1
// Opcode for Load Accumulator Indirect,Y (LDA)
#define INS_LDA_INDY 0xB1

// Opcode for Load X Immediate (LDX)
#define INS_LDX_IM 0xA2
// Opcode for Load X Zero Page (LDX)
#define INS_LDX_ZP 0xA6
// Opcode for Load X Zero Page,Y (LDX)
#define INS_LDX_ZPY 0xB6
// Opcode for Load X Absolute (LDX)
#define INS_LDX_AB 0xAE
// Opcode for Load X Absolute,Y (LDX)
#define INS_LDX_ABY 0xBE

// Opcode for Load Y Immediate (LDY)
#define INS_LDY_IM 0xA0
// Opcode for Load Y Zero Page (LDY)
#define INS_LDY_ZP 0xA4
// Opcode for Load Y Zero Page,X (LDY)
#define INS_LDY_ZPX 0xB4
// Opcode for Load Y Absolute (LDY)
#define INS_LDY_AB 0xAC
// Opcode for Load Y Absolute,X (LDY)
#define INS_LDY_ABX 0xBC

// Opcode for Store Accumulator Immediate (STA)
#define INS_STA_ZP 0x85
// Opcode for Store Accumulator Zero Page X (STA)
#define INS_STA_ZPX 0x95
// Opcode for Store Accumulator Absolute (STA)
#define INS_STA_AB 0x8D

// Opcode for Store Accumulator Absolute X (STA)
#define INS_STA_ABX 0x9D
// Opcode for Store Accumulator Absolute Y (STA)
#define INS_STA_ABY 0x99
// Opcode for Store Accumulator Indirect X (STA)
#define INS_STA_INDX 0x81
// Opcode for Store Accumulator Indirect Y (STA)
#define INS_STA_INDY 0x91

// Opcode for Store X Register Zero Page (STX)
#define INS_STX_ZP 0x86
// Opcode for Store X Register Zero Page,Y (STX)
#define INS_STX_ZPY 0x96
// Opcode for Store X Register Absolute (STX)
#define INS_STX_AB 0x8E

// Opcode for Store Y Register Zero Page (STY)
#define INS_STY_ZP 0x84
// Opcode for Store Y Register Zero Page,X (STY)
#define INS_STY_ZPX 0x94
// Opcode for Store Y Register Absolute (STY)
#define INS_STY_AB 0x8C

#endif