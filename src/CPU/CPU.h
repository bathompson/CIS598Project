#include <cstdint>
#include <functional>

#define INSTRUCTION_COUNT 0x100

class MemoryBus;

//typedef std::function<void()> Instruction;
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
        typedef void (CPU::*Instruction)();
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
        void OP_NOP();
        
        void OP_0x01();
        void OP_0x02();
        void OP_0x03();
        void OP_0x04();
        void OP_0x05();
        void OP_0x06();
        void OP_0x07();
        void OP_0x08();
        void OP_0x09();
        void OP_0x0A();
        void OP_0x0B();
        void OP_0x0C();
        void OP_0x0D();
        void OP_0x0E();
        void OP_0x0F();

        void OP_0x11();
        void OP_0x12();
        void OP_0x13();
        void OP_0x14();
        void OP_0x15();
        void OP_0x16();
        void OP_0x17();
        void OP_0x18();
        void OP_0x19();
        void OP_0x1A();
        void OP_0x1B();
        void OP_0x1C();
        void OP_0x1D();
        void OP_0x1E();
        void OP_0x1F();
};