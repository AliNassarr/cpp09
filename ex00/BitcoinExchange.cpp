#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other_exchange_object) : _historical_rates(other_exchange_object._historical_rates) {}
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& right_hand_side_object) { if (this != &right_hand_side_object) _historical_rates = right_hand_side_object._historical_rates; return *this; }
BitcoinExchange::~BitcoinExchange() {}

std::string BitcoinExchange::trim_whitespace(const std::string& string_to_trim) {
    std::string::size_type start_index = string_to_trim.find_first_not_of(" \t\r\n");
    return (start_index == std::string::npos) ? "" : string_to_trim.substr(start_index, string_to_trim.find_last_not_of(" \t\r\n") - start_index + 1);
}

bool BitcoinExchange::is_calendar_date_valid(const std::string& date_string_to_check) {
    if (date_string_to_check.length() != 10 || date_string_to_check[4] != '-' || date_string_to_check[7] != '-') return false;
    for (std::size_t index = 0; index < 10; ++index) if (index != 4 && index != 7 && !std::isdigit(static_cast<unsigned char>(date_string_to_check[index]))) return false;
    int year_value = std::atoi(date_string_to_check.substr(0, 4).c_str());
    int month_value = std::atoi(date_string_to_check.substr(5, 2).c_str());
    int day_value = std::atoi(date_string_to_check.substr(8, 2).c_str());
    if (year_value < 1000 || year_value > 9999 || month_value < 1 || month_value > 12 || day_value < 1 || day_value > 31) return false;
    if (month_value == 2) return day_value <= (((year_value % 4 == 0 && year_value % 100 != 0) || (year_value % 400 == 0)) ? 29 : 28);
    return (month_value == 4 || month_value == 6 || month_value == 9 || month_value == 11) ? day_value <= 30 : true;
}

bool BitcoinExchange::parse_numeric_value(const std::string& string_to_parse, double& parsed_double_result) {
    if (string_to_parse.empty() || string_to_parse[0] == '.' || string_to_parse[string_to_parse.length() - 1] == '.' || string_to_parse[0] == '+') return false;
    int decimal_point_count = 0;
    for (std::size_t index = (string_to_parse[0] == '-' ? 1 : 0); index < string_to_parse.length(); ++index) {
        if (string_to_parse[index] == '.') { if (++decimal_point_count > 1) return false; }
        else if (!std::isdigit(static_cast<unsigned char>(string_to_parse[index]))) return false;
    }
    char* string_to_double_end_pointer = NULL;
    parsed_double_result = std::strtod(string_to_parse.c_str(), &string_to_double_end_pointer);
    return (string_to_double_end_pointer != NULL && *string_to_double_end_pointer == '\0');
}

bool BitcoinExchange::load_historical_rates_from_csv(const std::string& database_file_path) {
    std::ifstream historical_rates_file_stream(database_file_path.c_str());
    if (!historical_rates_file_stream.is_open()) { std::cerr << "Error: could not open database file." << std::endl; return false; }
    std::string current_csv_line;
    if (!std::getline(historical_rates_file_stream, current_csv_line)) return false;
    while (std::getline(historical_rates_file_stream, current_csv_line)) {
        std::string::size_type comma_separator_position = current_csv_line.find(',');
        if (current_csv_line.empty() || comma_separator_position == std::string::npos) continue;
        std::string date_string = trim_whitespace(current_csv_line.substr(0, comma_separator_position));
        if (!is_calendar_date_valid(date_string)) continue;
        char* end_pointer = NULL;
        double exchange_rate_value = std::strtod(trim_whitespace(current_csv_line.substr(comma_separator_position + 1)).c_str(), &end_pointer);
        if (end_pointer != NULL && *end_pointer == '\0') _historical_rates[date_string] = exchange_rate_value;
    }
    return !_historical_rates.empty();
}

void BitcoinExchange::process_input_file(const std::string& input_file_path) const {
    std::ifstream user_input_file_stream(input_file_path.c_str());
    if (!user_input_file_stream.is_open()) { std::cerr << "Error: could not open file." << std::endl; return; }
    std::string current_input_line;
    if (!std::getline(user_input_file_stream, current_input_line) || trim_whitespace(current_input_line) != "date | value") { std::cerr << "Error: invalid input file header." << std::endl; return; }
    while (std::getline(user_input_file_stream, current_input_line)) {
        if (current_input_line.empty()) continue;
        std::string::size_type pipe_separator_position = current_input_line.find('|');
        if (pipe_separator_position == std::string::npos) { std::cerr << "Error: bad input => " << current_input_line << std::endl; continue; }
        std::string date_string = trim_whitespace(current_input_line.substr(0, pipe_separator_position));
        std::string bitcoin_quantity_string = trim_whitespace(current_input_line.substr(pipe_separator_position + 1));
        if (!is_calendar_date_valid(date_string)) { std::cerr << "Error: bad input => " << (date_string.empty() ? current_input_line : date_string) << std::endl; continue; }
        double bitcoin_quantity = 0.0;
        if (!parse_numeric_value(bitcoin_quantity_string, bitcoin_quantity)) { std::cerr << "Error: bad input => " << current_input_line << std::endl; continue; }
        if (bitcoin_quantity < 0.0) { std::cerr << "Error: not a positive number." << std::endl; continue; }
        if (bitcoin_quantity > 1000.0) { std::cerr << "Error: too large a number." << std::endl; continue; }
        std::map<std::string, double>::const_iterator closest_rate_iterator = _historical_rates.upper_bound(date_string);
        if (closest_rate_iterator == _historical_rates.begin()) { std::cerr << "Error: no data available for date " << date_string << std::endl; continue; }
        std::cout << date_string << " => " << bitcoin_quantity << " = " << (bitcoin_quantity * (--closest_rate_iterator)->second) << std::endl;
    }
}
