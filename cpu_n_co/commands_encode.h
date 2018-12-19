#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>

#include "commands.h"

#define SKYPROC_ENC_SIGNATURE(name) inline void SKYPROC_CMD_##name##_encode( \
    std::stringstream& ss, std::vector<word>& code, \
    const std::vector<int>& lineAddr, \
    const std::unordered_map<std::string, int>& labelLine, int addr)


#define SKYPROC_ENC_NOARG(name) \
    SKYPROC_ENC_SIGNATURE(name) { \
        (void)ss, (void)code, (void)lineAddr, (void)labelLine, (void)addr; \
    }

SKYPROC_ENC_NOARG(ADD)
SKYPROC_ENC_NOARG(SUB)
SKYPROC_ENC_NOARG(MUL)
SKYPROC_ENC_NOARG(DIV)
SKYPROC_ENC_NOARG(MOD)
SKYPROC_ENC_NOARG(RET)


#define SKYPROC_ENC_JMPLIKE(name) \
    SKYPROC_ENC_SIGNATURE(name) { \
        std::string label; \
        ss >> label; \
        for (char& c : label) c = toupper(c); \
        code.push_back(lineAddr[labelLine.at(label)] - addr); \
    }

SKYPROC_ENC_JMPLIKE(JMP)
SKYPROC_ENC_JMPLIKE(JEQ)
SKYPROC_ENC_JMPLIKE(JNE)
SKYPROC_ENC_JMPLIKE(JLT)
SKYPROC_ENC_JMPLIKE(JGT)
SKYPROC_ENC_JMPLIKE(JLE)
SKYPROC_ENC_JMPLIKE(JGE)
SKYPROC_ENC_JMPLIKE(CALL)


#define SKYPROC_PARSE_REG(var) \
    word var = 0xDEAD1337BEEF; \
    { \
        std::string token; \
        ss >> token; \
        std::stringstream ss2(token); \
        char c; \
        ss2 >> c >> var; \
        assert(c == 'r'); \
        assert(0 <= var && var < REGS_CNT); \
    }

SKYPROC_ENC_SIGNATURE(SET) {
    (void)lineAddr, (void)labelLine, (void)addr;

    SKYPROC_PARSE_REG(ri);
    word x;
    ss >> x;

    code.push_back(ri);
    code.push_back(x);
}

#define SKYPROC_ENC_REGARG(name) \
    SKYPROC_ENC_SIGNATURE(name) { \
        (void)lineAddr, (void)labelLine, (void)addr; \
        SKYPROC_PARSE_REG(ri); \
        code.push_back(ri); \
    }

SKYPROC_ENC_REGARG(PUSH)
SKYPROC_ENC_REGARG(POP)
SKYPROC_ENC_REGARG(LOAD)
SKYPROC_ENC_REGARG(STOR)
