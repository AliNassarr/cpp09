#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN {
public:
    RPN();
    RPN(const RPN& other);
    RPN& operator=(const RPN& rhs);
    ~RPN();

    int calculate(const std::string& expression);

private:
    std::stack<int> _operands;

    static bool isOperator(const std::string& token);
    static bool isDigit(const std::string& token);
    static int executeOperation(int left, int right, const std::string& op);
};

#endif
