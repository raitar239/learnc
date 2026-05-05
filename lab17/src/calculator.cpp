#include "calculator.h"
#include <iostream>

int calculator::addition(int value, int value2) {
    return value + value2;
}

int calculator::subtraction(int value, int value2) {
    return value - value2;
}

int calculator::division(int value, int value2) {
    if (value2 == 0) return 0; 
    return value / value2;
}

int calculator::multiplcation(int value, int value2) {
    return value * value2;
}