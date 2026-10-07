#ifndef RPN_HPP
#define RPN_HPP

#include <string>

class ReversePolishNotationCalculator {
public:
    ReversePolishNotationCalculator();
    ReversePolishNotationCalculator(const ReversePolishNotationCalculator& other_calculator_object);
    ReversePolishNotationCalculator& operator=(const ReversePolishNotationCalculator& right_hand_side_object);
    ~ReversePolishNotationCalculator();

    static int evaluate_mathematical_expression(const std::string& mathematical_expression_string);

private:
    static bool is_valid_mathematical_operator(char character_to_check);
    static int perform_arithmetic_operation(int left_operand, int right_operand, char mathematical_operator);
};

#endif
