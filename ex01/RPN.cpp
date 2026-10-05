#include "RPN.hpp"
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
    if (op == '+')
        return a + b;
    if (op == '-')
        return a - b;
    if (op == '*')
        return a * b;
    if (op == '/') {
        if (b == 0)
            throw std::runtime_error("Error");
        return a / b;
    }
    throw std::runtime_error("Error");
}

int RPN::calculate(const std::string& expression) {
    std::stack<int> s;
    std::istringstream iss(expression);
    std::string token;

    while (iss >> token) {
        if (token.length() == 1 && std::isdigit(static_cast<unsigned char>(token[0]))) {
            s.push(token[0] - '0');
        } else if (token.length() == 1 && isOperator(token[0])) {
            if (s.size() < 2)
                throw std::runtime_error("Error");

            int b = s.top();
            s.pop();
            int a = s.top();
            s.pop();

            s.push(applyOperation(a, b, token[0]));
        } else {
            throw std::runtime_error("Error");
        }
    }

    if (s.size() != 1)
        throw std::runtime_error("Error");

    return s.top();
}
