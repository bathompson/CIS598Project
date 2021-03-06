#include "MemoryBus.h"

uint8_t MemoryBus::memRead_8(uint16_t addr)
{
    return memory[addr];
}

uint16_t MemoryBus::memRead_16(uint16_t addr)
{
    return memory[addr+1] << 8 | memory[addr];
}

void MemoryBus::memWrite_8(uint16_t addr, uint8_t value)
{
    memory[addr] = value;

    //handle ECHO RAM block
    if(addr <= 0xDDFF && addr >= 0xC000)
    {
        memory[addr+0x2000] = value;
    }
    if(addr <= 0xFDFF && addr >= 0xE000)
    {
        memory[addr - 0x2000] = value;
    }
}

MemoryBus &MemoryBus::Instance()
{
    static MemoryBus instance;
    return instance;
}