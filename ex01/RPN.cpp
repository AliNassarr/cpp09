#include "RPN.hpp"
#include <sstream>
#include <stdexcept>
#include <cctype>

RPN::RPN() {}

RPN::RPN(const RPN& other) : _operands(other._operands) {}

RPN& RPN::operator=(const RPN& rhs) {
    if (this != &rhs) {
        _operands = rhs._operands;
    }
    return *this;
}

RPN::~RPN() {}

bool RPN::isOperator(const std::string& token) {
    return (token == "+" || token == "-" || token == "*" || token == "/");
}

bool RPN::isDigit(const std::string& token) {
    return (token.length() == 1 && std::isdigit(static_cast<unsigned char>(token[0])));
}

int RPN::executeOperation(int left, int right, const std::string& op) {
    if (op == "+")
        return left + right;
    if (op == "-")
        return left - right;
    if (op == "*")
        return left * right;
    if (op == "/") {
        if (right == 0)
            throw std::runtime_error("Error");
        return left / right;
    }
    throw std::runtime_error("Error");
}

int RPN::calculate(const std::string& expression) {
    while (!_operands.empty()) {
        _operands.pop();
    }

    std::istringstream stream(expression);
    std::string token;

    while (stream >> token) {
        if (isDigit(token)) {
            _operands.push(token[0] - '0');
        } else if (isOperator(token)) {
            if (_operands.size() < 2)
                throw std::runtime_error("Error");

            int right = _operands.top();
            _operands.pop();
            int left = _operands.top();
            _operands.pop();

            int result = executeOperation(left, right, token);
            _operands.push(result);
        } else {
            throw std::runtime_error("Error");
        }
    }

    if (_operands.size() != 1)
        throw std::runtime_error("Error");

    return _operands.top();
}
