#include <cassert>

#include "state.h"

#define UNUSED(var) (void)var

#define SKYPROC_EXEC_SIGNATURE(name) inline void SKYPROC_CMD_##name##_exec(const std::vector<word>& code, State& state)

#define SKYPROC_BINOP_EXEC(name, op_) \
    SKYPROC_EXEC_SIGNATURE(name) { \
        UNUSED(code); \
        assert(state.stack.size() >= 2); \
        word top = state.stack.top(); \
        state.stack.pop(); \
        word bot = state.stack.top(); \
        \
        state.stack.push(top op_ bot); \
        \
        state.ip++; \
    }

SKYPROC_BINOP_EXEC(ADD, +)
SKYPROC_BINOP_EXEC(SUB, -)
SKYPROC_BINOP_EXEC(MUL, *)
SKYPROC_BINOP_EXEC(DIV, /)
SKYPROC_BINOP_EXEC(MOD, %)


SKYPROC_EXEC_SIGNATURE(SET) {
    word i = code.at(state.ip + 1);
    word x = code.at(state.ip + 2);

    assert(0 <= i && i < REGS_CNT);
    state.reg[i] = x;

    state.ip += 3;
}

SKYPROC_EXEC_SIGNATURE(PUSH) {
    word i = code.at(state.ip + 1);
    
    assert(0 <= i && i < REGS_CNT);
    state.stack.push(state.reg[i]);

    state.ip += 2;
}

SKYPROC_EXEC_SIGNATURE(POP) {
    word i = code.at(state.ip + 1);

    assert(0 <= i && i < REGS_CNT);
    assert(state.stack.size() >= 1);
    state.reg[i] = state.stack.top();
    state.stack.pop();

    state.ip += 2;
}

SKYPROC_EXEC_SIGNATURE(STOR) {
    assert(state.stack.size() >= 1);
    word i = code.at(state.ip + 1);
    word x = state.stack.top();
    state.stack.pop();

    state.ram.at(state.reg[i]) = x;

    state.ip += 2;
}

SKYPROC_EXEC_SIGNATURE(LOAD) {
    word i = code.at(state.ip + 1);
    word x = state.ram.at(state.reg[i]);

    state.stack.push(x);

    state.ip += 2;
}


SKYPROC_EXEC_SIGNATURE(JMP) {
    word offset = code.at(state.ip + 1);
    state.ip += offset;
}

#define SKYPROC_JMP_EXEC(name, pred_) \
    SKYPROC_EXEC_SIGNATURE(name) { \
        assert(state.stack.size() >= 2); \
        word offset = code.at(state.ip + 1); \
        word top = state.stack.top(); \
        state.stack.pop(); \
        word bot = state.stack.top(); \
        state.stack.push(top); \
        if (top pred_ bot) { \
            state.ip += offset; \
        } else { \
            state.ip += 2; \
        } \
    }

SKYPROC_JMP_EXEC(JEQ, ==)
SKYPROC_JMP_EXEC(JNE, !=)
SKYPROC_JMP_EXEC(JLT, <)
SKYPROC_JMP_EXEC(JGT, >)
SKYPROC_JMP_EXEC(JLE, <=)
SKYPROC_JMP_EXEC(JGE, >=)


SKYPROC_EXEC_SIGNATURE(CALL) {
    word new_ip = code.at(state.ip + 1);
    state.stack.push(state.ip + 2);
    state.ip = new_ip;
}

SKYPROC_EXEC_SIGNATURE(RET) {
    UNUSED(code);
    assert(state.stack.size() >= 1);
    word new_ip = state.stack.top();
    state.stack.pop();
    state.ip = new_ip;
}

#undef UNUSED
