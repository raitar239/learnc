#include <gtest/gtest.h>
#include "../src/calculator.h"

TEST(CalculatorTest, Addition) {
    calculator calc;
    EXPECT_EQ(calc.addition(2, 2), 4);
    EXPECT_EQ(calc.addition(-1, 5), 4);
}

TEST(CalculatorTest, Subtraction) {
    calculator calc;
    EXPECT_EQ(calc.subtraction(10, 5), 5);
    EXPECT_EQ(calc.subtraction(0, 5), -5);
}

TEST(CalculatorTest, Multiplication) {
    calculator calc;
    EXPECT_EQ(calc.multiplcation(3, 3), 9);
    EXPECT_EQ(calc.multiplcation(-2, 4), -8);
}

TEST(CalculatorTest, Division) {
    calculator calc;
    EXPECT_EQ(calc.division(10, 2), 5);
    EXPECT_EQ(calc.division(5, 0), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}