#include "RPN.hpp"
#include <stack>
#include <sstream>
#include <stdexcept>
#include <cctype>

ReversePolishNotationCalculator::ReversePolishNotationCalculator() {}
ReversePolishNotationCalculator::ReversePolishNotationCalculator(const ReversePolishNotationCalculator& other_calculator_object) { (void)other_calculator_object; }
ReversePolishNotationCalculator& ReversePolishNotationCalculator::operator=(const ReversePolishNotationCalculator& right_hand_side_object) { (void)right_hand_side_object; return *this; }
ReversePolishNotationCalculator::~ReversePolishNotationCalculator() {}

bool ReversePolishNotationCalculator::is_valid_mathematical_operator(char character_to_check) {
    return (character_to_check == '+' || character_to_check == '-' || character_to_check == '*' || character_to_check == '/');
}

int ReversePolishNotationCalculator::perform_arithmetic_operation(int left_operand, int right_operand, char mathematical_operator) {
    if (mathematical_operator == '/' && right_operand == 0) throw std::runtime_error("Error");
    return mathematical_operator == '+' ? left_operand + right_operand :
           mathematical_operator == '-' ? left_operand - right_operand :
           mathematical_operator == '*' ? left_operand * right_operand : left_operand / right_operand;
}

int ReversePolishNotationCalculator::evaluate_mathematical_expression(const std::string& mathematical_expression_string) {
    std::stack<int> number_storage_stack;
    std::istringstream expression_token_stream(mathematical_expression_string);
    std::string current_string_token;

    while (expression_token_stream >> current_string_token) {
        if (current_string_token.length() == 1 && std::isdigit(static_cast<unsigned char>(current_string_token[0]))) {
            number_storage_stack.push(current_string_token[0] - '0');
        } else if (current_string_token.length() == 1 && is_valid_mathematical_operator(current_string_token[0])) {
            if (number_storage_stack.size() < 2) throw std::runtime_error("Error");
            int right_operand = number_storage_stack.top(); number_storage_stack.pop();
            int left_operand = number_storage_stack.top(); number_storage_stack.pop();
            number_storage_stack.push(perform_arithmetic_operation(left_operand, right_operand, current_string_token[0]));
        } else throw std::runtime_error("Error");
    }
    if (number_storage_stack.size() != 1) throw std::runtime_error("Error");
    return number_storage_stack.top();
}
