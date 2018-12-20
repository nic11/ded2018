#include <iostream>
#include <fstream>
#include <cassert>

#include "assembler.h"
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
    assert(argc == 3);

    std::ifstream fin(argv[1]);
    std::string prog;
    prog.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

    std::vector<word> code = encode(prog);
    // std::cout << "program len: " << code.size() << "\n";
    // for (word wrd : code) std::cout << wrd << ",\n";

    std::ofstream fout(argv[2], std::ostream::binary);
    for (word wrd : code) {
        fout.write(wordToBytes(wrd), 8);
    }
}
