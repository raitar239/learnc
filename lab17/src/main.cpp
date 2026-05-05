#include <iostream>
#include "calculator.h"

using std::cout;
using std::cin;
using std::endl;

int main()
{
    int value;
    int value2;
    char operation; 
    char userAnswer;
    calculator calc;

    do
    {
        cout << "Enter equation with only two values (Example: 2 + 5): " << endl;
        if (!(cin >> value >> operation >> value2)) {
            cout << "Invalid input!" << endl;
            break;
        }

        int result = 0;
        switch(operation)
        {
        case '+':
            result = calc.addition(value, value2);
            break;
        case '-':
            result = calc.subtraction(value, value2);
            break;
        case '/':
            result = calc.division(value, value2);
            break;
        case '*':
            result = calc.multiplcation(value, value2);
            break;
        default:
            cout << "Unknown operation!";
            continue;
        }

        cout << "Result: " << result << endl;

        cout << "\n Would you like to input another equation (Y/N): ";
        cin >> userAnswer;

    } while (userAnswer == 'Y' || userAnswer == 'y');

    return 0;
}