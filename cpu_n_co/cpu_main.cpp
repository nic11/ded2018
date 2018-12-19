#include <iostream>

#include "cpu.h"
#include "commands_defs.h"

int main() {
    std::vector<word> code {
        82543837201,
0,
2,
82543837202,
0,
82543837202,
0,
82543837201,
1,
0,
82543837201,
2,
11,
82543837201,
3,
1,
82543837202,
2,
82543837202,
1,
82543837203,
1,
82543837203,
2,
82543837187,
82543837202,
2,
82543837202,
3,
82543837202,
1,
82543837185,
82543837203,
1,
82543837203,
3,
82543837202,
1,
82543837220,
-18,
    };

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
    std::cout << "\n";

    // std::cout << "ram:\n";
    // for (int i = 0; i < 
}
