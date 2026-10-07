#include "BitcoinExchange.hpp"
#include <iostream>

int main(int command_line_argument_count, char** command_line_arguments) {
    if (command_line_argument_count != 2) { std::cerr << "Error: could not open file." << std::endl; return 1; }
    BitcoinExchange bitcoin_exchange_calculator;
    if (!bitcoin_exchange_calculator.load_historical_rates_from_csv("data.csv")) { std::cerr << "Error: could not load database." << std::endl; return 1; }
    bitcoin_exchange_calculator.process_input_file(command_line_arguments[1]);
    return 0;
}
