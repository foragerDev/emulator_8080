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
        add(state, (uint16_t)state->c);
    }

    case 0x81: // ADD C SSS=001 form
    {
        add(state, (uint16_t)state->c);
    }
    case 0x86: // ADD M Load data from hight level and low level register
    {
        uint16_t offset = get_memory_offset(state); // combine both and you can access maximum of 65535 bytes of memory
        add(state, state->memory[offset]);
    }
    case 0x8E: // ADC M C(Add memory with carry) ADC ((H) (L)) + (CY)
    {
        uint16_t offset = get_memory_offset(state);
        uint16_t answer = state->memory[offset] + (uint16_t)state->cc.cy;
        add(state, answer);
    }
    case 0x90: // SUB B (Subtract from Register B(000))
    {
        subtract(state, state->b);
    }
    case 0x91: // SUB C
    {
        subtract(state, state->c);
    }
    case 0x96: // SUB M (Subtract from memory)
    {
        uint16_t offset = get_memory_offset(state);
        subtract(state, state->memory[offset]);
    }
    case 0x98: // SBB B(sbtract from register and carry flag)
    {
        uint16_t answer = state->b - state->cc.cy;
        subtract(state, answer);
    }
    case 0x99: // SBB C
    {
        uint16_t answer = state->c - state->cc.cy;
        subtract(state, answer);
    }
    case 0x9E: // Subtract memory with borrow
    {
        uint16_t offset = get_memory_offset(state);
        u_int16_t answer = state->memory[offset] + state->cc.cy;
        subtract(state, answer);
    }
    case 0xC6: // ADD byte (immidetate form)
    {
        add(state, (uint16_t)opcode[1]);
    }
    case 0xCE: // ACI data (add immideate with carry) A <- (A) + (2nd byte) + (CY)
    {
        uint16_t answer = (uint16_t)opcode[1] + uint16_t(state->cc.cy);
        add(state, answer);
    }
    case 0xD6: // SUI data (substract immideate 2nd byte)
    {
        subtract(state, (uint16_t)opcode[1]);
    }
    case 0xDE: // SBI
    {
        subtract(state, (uint16_t)opcode[1] - state->cc.cy);
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
