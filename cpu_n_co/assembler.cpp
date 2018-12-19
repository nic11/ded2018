#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <unordered_map>

#include "assembler.h"
#include "commands_encode.h"

int firstPass(const std::string& prog, std::vector<int>& lineAddr, std::unordered_map<std::string, int>& labelLine) {
    lineAddr.reserve(std::count(prog.begin(), prog.end(), '\n'));

    std::stringstream ss(prog);
    std::string token;
    int line = 0, addr = 0;
    while (ss >> token) {
        for (char& c: token) c = toupper(c);

        if (token.back() == ':') {
            token.pop_back();
            lineAddr.push_back(addr);
            labelLine[token] = line;
        } else {
        #undef SKYPROC_DEF_CMD
        #define SKYPROC_DEF_CMD(name, opcode, width) \
            if (token == #name) { \
                for (int i = 1; i < width; ++i) { \
                    ss >> token; \
                } \
                lineAddr.push_back(addr); \
                addr += width; \
            } else

        #include "commands_defs.h"
            /* else */ {
                assert(false && "Unknown command");
            }
        }

        ++line;
    }

    return addr;
}

std::vector<word> encode(const std::string& prog) {
    std::vector<int> lineAddr;
    std::unordered_map<std::string, int> labelLine;
    int size = firstPass(prog, lineAddr, labelLine);

    std::vector<word> code;
    code.reserve(size);

    std::stringstream ss(prog);
    std::string token;
    int line = 0, addr = 0;
    while (ss >> token) {
        for (char& c: token) c = toupper(c);

        if (token.back() == ':') {
            token.pop_back();
            // labelLine[token] = line;
        } else {
        #undef SKYPROC_DEF_CMD
        #define SKYPROC_DEF_CMD(name, opcode, width) \
            if (token == #name) { \
                /*std::cout << line << "@" << addr << "[" << lineAddr[line] << "]: " << token << " "; \
                for (int i = 1; i < width; ++i) { \
                    ss >> token; \
                    std::cout << token << " "; \
                } \
                std::cout << "\n";*/ \
                code.push_back(opcode); \
                SKYPROC_CMD_##name##_encode(ss, code, lineAddr, labelLine, addr); \
                assert(lineAddr[line] == addr); \
                addr += width; \
            } else

        #include "commands_defs.h"
            /* else */ {
            }
        }

        ++line;
    }

    assert((int)code.size() == size);

    return code;
}
