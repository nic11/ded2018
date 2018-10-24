#include <cassert>
#include <cmath>
#include <utility>

#include "solve_square.h"

namespace nic11 {
    bool equal(double a, double b, double eps) {
        return fabs(a - b) < eps;
    }

    int solveSqureEquation(double a, double b, double c, double* x1, double* x2) {
        if (!std::isfinite(a) ||
                !std::isfinite(b) ||
                !std::isfinite(c) ||
                x1 == nullptr ||
                x2 == nullptr ||
                x1 == x2) {
            throw TypeError();
        }

        if (equal(a, 0)) {
            // bx + c = 0
            if (equal(b, 0)) {
                // c = 0
                if (equal(c, 0)) {
                    return INF_ROOTS;
                }

                return 0;
            } else {
                *x1 = -c / b;
                return 1;
            }
        } else {
            double d = b * b - 4 * a * c;
            if (equal(d, 0)) {
                *x1 = -b / 2 / a;
                return 1;
            } else if (d < 0) {
                return 0;
            } else {
                double d12 = sqrt(d);
                *x1 = (-b - d12) / (2 * a);
                *x2 = (-b + d12) / (2 * a);
                if (*x1 > *x2) {
                    std::swap(*x1, *x2);
                }
                return 2;
            }
        }
    }
}
