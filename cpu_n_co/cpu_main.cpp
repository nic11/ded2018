#include <iostream>
#include <cassert>
#include <fstream>

#include "cpu.h"
#include "commands_defs.h"

word bytesToWord(char* b) {
    unsigned long long w = 0;
    for (int i = 0; i < 8; ++i) {
        w <<= 8;
        w |= (unsigned char)b[i];
    }
    return (word)w;
}

char* wordToBytes(word w) {
    static_assert(sizeof(w) == 8, "word should be 64-bit");
    static unsigned char c[8];
    for (int i = 7; i >= 0; --i) {
        c[7 - i] = ((unsigned long long)w >> (8 * i)) & 0xff;
    }
    assert(bytesToWord((char*)c) == w);
    return (char*)c;
}

int main(int argc, char** argv) {
    assert(argc == 2);
    std::vector<word> code;
    std::ifstream fin(argv[1], std::ostream::binary);

    while (true) {
        char c[8];
        fin.read(c, 8);
        if (fin.eof()) break;

        code.push_back(bytesToWord(c));
    }

    State state;
    exec(code, state);

    std::cout << "registers:\n";
    for (int i = 0; i < REGS_CNT; ++i) {
        std::cout << "  r" << i << ": " << state.reg[i] << "\n";
    }
    std::cout << "\n";

    std::cout << "stack (top first):\n";
    if (!state.stack.size()) {
        std::cout << "  (empty)\n";
    }
    while (state.stack.size()) {
        std::cout << "  " << state.stack.top() << "\n";
        state.stack.pop();
    }
}
