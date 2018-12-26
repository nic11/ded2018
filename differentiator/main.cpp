#include <iostream>

#include "expr.h"
#include "rpn.h"
#include "differentiate.h"

int main(int argc, char** argv) {
    std::string s;
    std::getline(std::cin, s);

    auto n = treeFromRpn(s);
    std::cout << "expr (rpn): " << treeToRpn(n) << "\n";
    n = differentiate(n, 1);
    std::cout << "derivative (rpn): " << treeToRpn(n) << "\n";

    bool didSomething;
    while (true) {
        std::tie(n, didSomething) = simplify(n);
        if (!didSomething) break;
        std::cout << "simplified derivative (rpn): " << treeToRpn(n) << "\n";
    }

    return 0;
}
