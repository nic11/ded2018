#include <cassert>

#include "cpu.h"
#include "commands_impl.h"

void tick(const std::vector<word>& code, State& state) {
#undef SKYPROC_DEF_CMD
#define SKYPROC_DEF_CMD(name, opcode, width) \
    if (code[state.ip] == opcode) { \
        assert(state.ip + width <= (int)code.size()); \
        SKYPROC_CMD_##name##_exec(code, state); \
    } else

#include "commands_defs.h"
    /* else */ {
        assert("Invalid instruction" && false);
    }
}

void exec(const std::vector<word>& code, State& state) {
    state.ram.resize(1 << 20); // 1 MB
    state.ip = 0;

    word prev_ip = -1;

    while (state.ip != (int)code.size()) {
        assert(0 <= state.ip && state.ip < (int)code.size());
        assert(state.ip != prev_ip);
        prev_ip = state.ip;

        tick(code, state);
    }
}
