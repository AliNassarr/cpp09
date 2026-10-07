#ifndef RPN_HPP
#define RPN_HPP

#include <string>

class RPN {
public:
    RPN();
    RPN(const RPN& other);
    RPN& operator=(const RPN& rhs);
    ~RPN();

    static int calculate(const std::string& expression);

private:
    static bool isOperator(char c);
    static int applyOperation(int a, int b, char op);
};

#endif
