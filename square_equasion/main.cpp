#include <iostream>

#include "solve_square.h"

const int EXIT_BAD_INPUT = 1;
const int EXIT_OTHER_ERROR = 127;

int main() {
    std::cout << "  Please enter a, b and c for square equation\n";
    std::cout << "  Example: 1 0 -1\n\n";
    
    std::cout << "> ";
    double a, b, c;
    std::cin >> a >> b >> c;

    if (std::cin.fail()) {
        std::cerr << "Bad input: 3 numbers expected\n";
        return EXIT_BAD_INPUT;
    }

    double x1, x2;
    int rootNum = nic11::solveSqureEquation(a, b, c, &x1, &x2);

    switch (rootNum) {
    case 0:
        std::cout << "No real roots present\n";
        break;
    case 1:
        std::cout << "One real root: " << x1 << "\n";
        break;
    case 2:
        std::cout << "Two real roots: " << x1 << " and " << x2 << "\n";
        break;
    case nic11::INF_ROOTS:
        std::cout << "Infinite real roots number\n";
        break;
    default:
        std::cerr << "Unknown return code of solveSquareEquation: " << rootNum << "\n";
        return EXIT_OTHER_ERROR;
    }

    return EXIT_SUCCESS;
}