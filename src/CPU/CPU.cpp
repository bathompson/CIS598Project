#include "CPU.h"
#include "MemoryBus/MemoryBus.h"

#define FLAG_C 1<<4
#define FLAG_H 1<<5
#define FLAG_N 1<<6
#define FLAG_Z 1<<7

bool halfCarryCheck_8(uint8_t a, uint8_t b)
{
    return (((a & 0xF) + (b & 0xF)) & 0x10) == 0x10;
}

bool halfCarryCheck_16(uint16_t a, uint16_t b)
{
    return (((a & 0xFF) + (b & 0xFF)) & 0x100) == 0x100;
}

CPU::CPU()
 : memoryBus(MemoryBus::Instance())
{
    usePrefix = false;
    cpuRegisters.AF = 0x01B0;
    cpuRegisters.BC = 0x0013;
    cpuRegisters.DE = 0x00D8;
    cpuRegisters.HL = 0x014D;
    PC = 0x100;
    SP = 0xFFFE;
}

void CPU::cycle()
{
    if(clocksRemaining <= 0)
    {
        uint8_t opcode = memoryBus.memRead_8(PC++);
        if(usePrefix)
            prefixInstructions[opcode];
        else
            instructsions[opcode];
    }
    clocksRemaining--;
}

bool CPU::isFlagSet(uint8_t flag)
{
    return cpuRegisters.F & flag;
}

void CPU::setFlag(uint8_t flag, bool value)
{
    if(value)
    {
        cpuRegisters.F |= flag;
    }
    else
    {
        cpuRegisters.F &= ~flag;
    }
}

//Instructions

//NOP - Do nothing other than cycle
void CPU::OP_NOP()
{
    clocksRemaining = 1;
}

//LD BC, d16
void CPU::OP_0x01()
{
    clocksRemaining = 3;
    cpuRegisters.BC = MemoryBus::Instance().memRead_16(PC);
    PC += 2;
}

//LD (BC), A
void CPU::OP_0x02()
{
    clocksRemaining = 2;
    MemoryBus::Instance().memWrite_8(cpuRegisters.BC, cpuRegisters.A);
}

//INC BC
void CPU::OP_0x03()
{
    clocksRemaining = 2;
    cpuRegisters.BC++;
}

//INC B
void CPU::OP_0x04()
{
    clocksRemaining = 1;
    setFlag(FLAG_N, false);
    setFlag(FLAG_H, halfCarryCheck_8(cpuRegisters.B, 1));
    cpuRegisters.B++;
    setFlag(FLAG_Z, cpuRegisters.B == 0);
}

//DEC B
void CPU::OP_0x05()
{
    clocksRemaining = 1;
    setFlag(FLAG_N, true);
    setFlag(FLAG_H, halfCarryCheck_8(cpuRegisters.B, ~1));
    cpuRegisters.B--;
    setFlag(FLAG_Z, cpuRegisters.B == 0);
}

//LD B, u8
void CPU::OP_0x06()
{
    clocksRemaining = 2;
    cpuRegisters.B = MemoryBus::Instance().memRead_8(PC++);
}

//RLCA
void CPU::OP_0x07()
{
    clocksRemaining = 2;
    uint8_t bit7 = (cpuRegisters.A & 1<<7)>>7;
    cpuRegisters.A = (cpuRegisters.A << 1) | bit7;
    setFlag(FLAG_C, bit7 > 0);
    setFlag(FLAG_H, false);
    setFlag(FLAG_N, false);
    setFlag(FLAG_Z, false);
}

//LD (a16), SP
void CPU::OP_0x08()
{
    clocksRemaining = 5;
    uint16_t addr = MemoryBus::Instance().memRead_16(PC);
    PC+=2;
    uint8_t spLower = SP;
    uint8_t spUpper = SP>>8;
    MemoryBus::Instance().memWrite_8(addr, spLower);
    MemoryBus::Instance().memWrite_8(addr+1, spUpper);
}

//ADD HL, BC
void CPU::OP_0x09()
{
    clocksRemaining = 2;
    uint32_t sum = cpuRegisters.HL + cpuRegisters.BC;
    setFlag(FLAG_C, sum & 0x10000 == 0x10000);
    setFlag(FLAG_H, halfCarryCheck_16(cpuRegisters.HL, cpuRegisters.BC));
    setFlag(FLAG_N, false);
    cpuRegisters.HL = (uint16_t) sum;
}

//LD A, (BC)
void CPU::OP_0x0A()
{
    clocksRemaining = 2;
    cpuRegisters.A = MemoryBus::Instance().memRead_8(cpuRegisters.BC);
}

//DEC BC
void CPU::OP_0x0B()
{
    clocksRemaining = 2;
    cpuRegisters.BC--;
}

//INC C
void CPU::OP_0x0C()
{
    clocksRemaining = 1;
    setFlag(FLAG_N, false);
    setFlag(FLAG_H, halfCarryCheck_8(cpuRegisters.C, ~1));
    cpuRegisters.C++;
    setFlag(FLAG_Z, cpuRegisters.C == 0);
}

//DEC C
void CPU::OP_0x0D()
{
    clocksRemaining = 1;
    setFlag(FLAG_N, true);
    setFlag(FLAG_H, halfCarryCheck_8(cpuRegisters.C, ~1));
    cpuRegisters.C--;
    setFlag(FLAG_Z, cpuRegisters.C == 0);
}

//LD C, d8
void CPU::OP_0x0E()
{
    clocksRemaining = 2;
    cpuRegisters.C = MemoryBus::Instance().memRead_8(PC++);
}

//RRCA
void CPU::OP_0x0F()
{
    clocksRemaining = 2;
    uint8_t bit0 = cpuRegisters.A & 0x01;
    cpuRegisters.A = (cpuRegisters.A >> 1) | (bit0 << 7);
    setFlag(FLAG_C, bit0 > 0);
    setFlag(FLAG_H, false);
    setFlag(FLAG_N, false);
    setFlag(FLAG_Z, false);
}