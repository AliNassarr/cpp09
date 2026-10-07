#include "RPN.hpp"
#include <stack>
#include <sstream>
#include <stdexcept>
#include <cctype>

RPN::RPN() {}

RPN::RPN(const RPN& other) {
    (void)other;
}

RPN& RPN::operator=(const RPN& rhs) {
    (void)rhs;
    return *this;
}

RPN::~RPN() {}

bool RPN::isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int RPN::applyOperation(int a, int b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') {
        if (b == 0) {
            throw std::runtime_error("Error");
        }
        return a / b;
    }
    throw std::runtime_error("Error");
}

int RPN::calculate(const std::string& expression) {
    std::stack<int> stack;
    std::istringstream iss(expression);
    std::string token;

    while (iss >> token) {
        // If it's a single digit, convert to int and push to stack
        if (token.length() == 1 && std::isdigit(token[0])) {
            int number = token[0] - '0';
            stack.push(number);
        } 
        // If it's an operator, pop the last two numbers and apply it
        else if (token.length() == 1 && isOperator(token[0])) {
            if (stack.size() < 2) {
                throw std::runtime_error("Error");
            }
            
            int b = stack.top();
            stack.pop();
            
            int a = stack.top();
            stack.pop();
            
            int result = applyOperation(a, b, token[0]);
            stack.push(result);
        } 
        // If it's anything else, it's invalid
        else {
            throw std::runtime_error("Error");
        }
    }

    // After processing everything, there should be exactly one result left
    if (stack.size() != 1) {
        throw std::runtime_error("Error");
    }

    return stack.top();
}
