#include <iostream>
#include <fstream>

#include "assembler.h"
#include "commands_defs.h"

int main(int argc, char** argv) {
    std::ifstream fin(argv[1]);
    std::string prog;
    prog.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

    std::vector<word> code = encode(prog);
    std::cout << "program len: " << code.size() << "\n";
    for (word wrd : code) std::cout << wrd << ",\n";
}
