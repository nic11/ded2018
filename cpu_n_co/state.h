#pragma once

#include <stack>
#include <vector>

typedef long long word;

const int REGS_CNT = 4;

struct State {
    std::stack<word> stack;
    std::vector<word> ram;
    word reg[REGS_CNT];
    word ip;
};
