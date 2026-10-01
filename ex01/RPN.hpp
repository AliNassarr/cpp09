#ifndef RPN_HPP
#define RPN_HPP

#include <string>

class RPN
{
public:
	static int calculate(const std::string& expression);

private:
	RPN();
	RPN(const RPN& other);
	RPN& operator=(const RPN& rhs);
	~RPN();

	static bool _isOperator(char c);
	static int _applyOperation(int a, int b, char op);
};

#endif
