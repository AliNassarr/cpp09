#include "RPN.hpp"
#include <stack>
#include <sstream>
#include <stdexcept>
#include <cctype>

RPN::RPN()
{
}

RPN::RPN(const RPN& other)
{
	(void)other;
}

RPN& RPN::operator=(const RPN& rhs)
{
	(void)rhs;
	return *this;
}

RPN::~RPN()
{
}

bool RPN::_isOperator(char c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/');
}

int RPN::_applyOperation(int a, int b, char op)
{
	switch (op)
	{
		case '+':
			return a + b;
		case '-':
			return a - b;
		case '*':
			return a * b;
		case '/':
			if (b == 0)
				throw std::runtime_error("division by zero");
			return a / b;
		default:
			throw std::runtime_error("unknown operator");
	}
}

int RPN::calculate(const std::string& expression)
{
	std::stack<int> stack;
	std::istringstream stream(expression);
	std::string token;

	while (stream >> token)
	{
		if (token.length() == 1 && _isOperator(token[0]))
		{
			if (stack.size() < 2)
				throw std::runtime_error("insufficient operands");

			int right = stack.top();
			stack.pop();
			int left = stack.top();
			stack.pop();

			int result = _applyOperation(left, right, token[0]);
			stack.push(result);
		}
		else if (token.length() == 1 && std::isdigit(static_cast<unsigned char>(token[0])))
		{
			stack.push(token[0] - '0');
		}
		else
		{
			throw std::runtime_error("invalid token");
		}
	}

	if (stack.size() != 1)
		throw std::runtime_error("invalid RPN expression");

	return stack.top();
}
