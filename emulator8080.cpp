#include <stdint.h>
#include <iostream>
#include "instructions.h"


using namespace cpu;

int Emulate8080p(State8080 *state)
{
    unsigned char *opcode = &state->memory[state->pc];
    switch (*opcode)
    {
    case 0x00:
        UnimplementedInstruction(state);
        break;
    case 0x01:
        state->c = opcode[1];
        state->b = opcode[2];
        state->pc += 2;
        break;
    case 0x02:
        UnimplementedInstruction(state);
        break;
    case 0x03:
        UnimplementedInstruction(state);
        break;
    case 0x80: // ADD B SSS=000 form
    {
        uint16_t answer = (uint16_t)state->a + (uint16_t)state->b;
        add(state, answer);
    }

    case 0x81: // ADD C SSS=001 form
    {
        uint16_t answer = (uint16_t)state->a + (uint16_t)state->b;
        add(state, answer);
    }
    case 0x86: // ADD M Load data from hight level and low level register
    {
        uint16_t offset = get_memory_offset(state); // combine both and you can access maximum of 65535 bytes of memory
        uint16_t answer = (uint16_t)state->a + state->memory[offset];
        add(state, answer);
    }
    case 0x8E: // ADC M (Add memory with carry) ADC ((H) (L)) + (CY)
    {
        uint16_t offset = get_memory_offset(state);
        uint16_t answer = (uint16_t)state->a + state->memory[offset] + (uint16_t)state->cc.cy;
        add(state, answer);
    }
    case 0x90: // SUB B (Subtract from Register B(000))
    {
        uint16_t answer = state->a - state->b;
    }
    case 0xC6: // ADD byte (immidetate form)
    {
        uint16_t answer = (uint16_t)state->a + (uint16_t)opcode[1];
        add(state, answer);
    }
    case 0xCE: // ACI data (add immideate with carry) A <- (A) + (2nd byte) + (CY)
    {
        uint16_t answer = (uint16_t)state->a + (uint16_t)opcode[1] + uint16_t(state->cc.cy);
        add(state, answer);
    }

    /* */
    case 0xfe:
        UnimplementedInstruction(state);
        break;
    case 0xff:
        UnimplementedInstruction(state);
        break;

    default:
        break;
    }

    state->pc += 1;
}
