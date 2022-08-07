#ifndef INSTRUCTION
#define INSTRUCTION

#include <stdint.h>
#include <iostream>

namespace cpu
{
    struct ConditionCodes
    {
        uint8_t z : 1;
        uint8_t s : 1;
        uint8_t p : 1;
        uint8_t cy : 1;
        uint8_t ac : 1;
        uint8_t pad : 3;
    };

    struct State8080
    {
        uint8_t a; // register accumlator
        uint8_t b;
        uint8_t c;
        uint8_t d;
        uint8_t e;
        uint8_t h;
        uint8_t l;
        uint16_t sp;
        uint16_t pc;
        uint8_t *memory;
        ConditionCodes cc;
        uint8_t int_enable;
    };

    void add(State8080 *state, uint16_t answer)
    {
        state->cc.z = ((answer & 0xff) == 0);          // is zero flag
        state->cc.s = ((answer & 0x80) != 1);          // set sign bit
        state->cc.cy = (answer & 0xff);                // carry flag
        state->cc.p = __builtin_parity(answer & 0xff); // parity flag
        state->a = answer & 0xff;
    }

    uint16_t get_memory_offset(State8080 *state)
    {
        return (state->h << 8) | (state->l);
    }
    void UnimplementedInstruction(State8080 *state)
    {
        std::cout << "Error: Unimplemented Instruction" << std::endl;
        exit(1);
    }

}

#endif // INSTRUCTION