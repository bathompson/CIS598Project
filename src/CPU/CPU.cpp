#include "CPU.h"
#include "MemoryBus/MemoryBus.h"

#define FLAG_C 1<<4
#define FLAG_H 1<<5
#define FLAG_N 1<<6
#define FLAG_Z 1<<7

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