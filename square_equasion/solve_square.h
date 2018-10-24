namespace nic11 {
    const double EPS = 1e-9;
    const int INF_ROOTS = -1;

    class TypeError {};

    bool equal(double a, double b, double eps = EPS);
    int solveSqureEquation(double a, double b, double c, double* x1, double* x2);
}
