#include <cstdint>

#define ADDRESSABLE_MEMORY_SIZE 0x10000

class MemoryBus
{
    private:
        uint8_t memory[ADDRESSABLE_MEMORY_SIZE]={0};
        MemoryBus() {};
        MemoryBus(const MemoryBus&) = delete;
        MemoryBus &operator=(const MemoryBus&) = delete;
    
    public:
        uint8_t memRead_8(uint16_t addr);
        void memWrite_8(uint16_t addr, uint8_t val);
        uint16_t memRead_16(uint16_t arddr);
        static MemoryBus &Instance();
        
};