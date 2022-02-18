#include <cstdint>
#include <functional>

#define INSTRUCTION_COUNT 0x100

class MemoryBus;

typedef std::function<void()> Instruction;

typedef struct registers
{
    union
    {
        struct
        {
            uint8_t F;
            uint8_t A;
        };
        uint16_t AF;
    };

    union
    {
        struct
        {
            uint8_t C;
            uint8_t B;
        };
        uint16_t BC;
    };

    union
    {
        struct
        {
            uint8_t E;
            uint8_t D;
        };
        uint16_t DE;
    };

    union
    {
        struct
        {
            uint8_t L;
            uint8_t H;
        };
        uint16_t HL;
    };
    
} Registers;
class CPU
{
    public:
        CPU();
        void cycle();
    private:
        int clocksRemaining=0;
        uint16_t PC;
        uint16_t SP;
        bool usePrefix;
        Instruction instructsions[INSTRUCTION_COUNT] = {OP_NOP};
        Instruction prefixInstructions[INSTRUCTION_COUNT] = {OP_NOP};
        MemoryBus &memoryBus;
        Registers cpuRegisters;
        bool isFlagSet(uint8_t flag);
        void setFlag(uint8_t flag, bool value);
        //Instructions
        Instruction OP_NOP  = [&](){clocksRemaining = 1;};
};