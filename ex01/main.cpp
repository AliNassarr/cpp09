#include "RPN.hpp"
#include <iostream>

int main(int command_line_argument_count, char** command_line_arguments) {
    if (command_line_argument_count != 2) { std::cerr << "Error" << std::endl; return 1; }
    try {
        std::cout << ReversePolishNotationCalculator::evaluate_mathematical_expression(command_line_arguments[1]) << std::endl;
    } catch (...) {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    return 0;
}
