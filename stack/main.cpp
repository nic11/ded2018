#include <iostream>
#include <cassert>

#include "armystack.h"

using std::cout;

int main() {
    ArmyStack s;
    s.mSize = -1;
    for (int i = 1; i <= 55; ++i) {
        for (int j = 0; j < i; ++j) {
            s.push(j);
        }
        for (int j = i - 1; j >= 0; --j) {
            assert(s.pop() == j);
        }
    }
    **(int**)&s = 7;
    s.push(228);
    **(int**)&s = 7;
    s.push(229);
    s.pop();
    s.pop();
    s.pop();
    s.pop();
    s.pop();
}
