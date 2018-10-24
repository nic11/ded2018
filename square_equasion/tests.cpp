#include <gtest/gtest.h>
#include <limits>
#include "solve_square.h"

TEST(solveSquareTest, squareTwoRoots) {
    double x1, x2;
    ASSERT_EQ(nic11::solveSqureEquation(1, 0, -1, &x1, &x2), 2);
    ASSERT_DOUBLE_EQ(x1, -1);
    ASSERT_DOUBLE_EQ(x2, 1);
}

TEST(solveSquareTest, squareOneRoot) {
    double x1, x2;
    ASSERT_EQ(nic11::solveSqureEquation(1, -6, 9, &x1, &x2), 1);
    ASSERT_DOUBLE_EQ(x1, 3);
}

TEST(solveSquareTest, squareNoRoots) {
    double x1, x2;
    ASSERT_EQ(nic11::solveSqureEquation(1, 1, 5, &x1, &x2), 0);
}

TEST(solveSquareTest, linear) {
    double x1, x2;
    ASSERT_EQ(nic11::solveSqureEquation(0, 1, 5, &x1, &x2), 1);
    ASSERT_DOUBLE_EQ(x1, -5);
}

TEST(solveSquareTest, degenerateInfiniteRoots) {
    double x1, x2;
    ASSERT_EQ(nic11::solveSqureEquation(0, 0, 0, &x1, &x2), nic11::INF_ROOTS);
}

TEST(solveSquareTest, degenerateNoRoots) {
    double x1, x2;
    ASSERT_EQ(nic11::solveSqureEquation(0, 0, 5, &x1, &x2), 0);
}

TEST(solveSquareExceptionTest, throwsOnBadPointers) {
    double x1, x2;
    EXPECT_THROW({ nic11::solveSqureEquation(1, 2, 3, &x1, nullptr); }, nic11::TypeError);
    EXPECT_THROW({ nic11::solveSqureEquation(1, 2, 3, nullptr, &x2); }, nic11::TypeError);
    EXPECT_THROW({ nic11::solveSqureEquation(1, 2, 3, nullptr, nullptr); }, nic11::TypeError);
    EXPECT_THROW({ nic11::solveSqureEquation(1, 2, 3, &x1, &x1); }, nic11::TypeError);
}

TEST(solveSquareExceptionTest, throwsOnBadNumbers) {
    const auto lims = std::numeric_limits<double>();
    const auto badNums = {lims.infinity(), lims.quiet_NaN(), lims.signaling_NaN()};

    double x1, x2;

    for (double bad : badNums) {
        EXPECT_THROW({ nic11::solveSqureEquation(bad, 2, 3, &x1, &x2); }, nic11::TypeError);
        EXPECT_THROW({ nic11::solveSqureEquation(1, bad, 3, &x1, &x2); }, nic11::TypeError);
        EXPECT_THROW({ nic11::solveSqureEquation(1, 2, bad, &x1, &x2); }, nic11::TypeError);
    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
