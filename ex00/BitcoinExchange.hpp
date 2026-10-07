#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange {
public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other_exchange_object);
    BitcoinExchange& operator=(const BitcoinExchange& right_hand_side_object);
    ~BitcoinExchange();

    bool load_historical_rates_from_csv(const std::string& database_file_path);
    void process_input_file(const std::string& input_file_path) const;

private:
    std::map<std::string, double> _historical_rates;

    static std::string trim_whitespace(const std::string& string_to_trim);
    static bool is_calendar_date_valid(const std::string& date_string_to_check);
    static bool parse_numeric_value(const std::string& string_to_parse, double& parsed_double_result);
};

#endif
