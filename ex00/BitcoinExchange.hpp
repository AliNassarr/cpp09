#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange {
public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& rhs);
    ~BitcoinExchange();

    bool loadDatabase(const std::string& filename);
    void processInput(const std::string& filename) const;

private:
    std::map<std::string, double> _db;

    static std::string trim(const std::string& str);
    static bool isValidDate(const std::string& date);
    static bool parseValue(const std::string& str, double& value);
};

#endif
