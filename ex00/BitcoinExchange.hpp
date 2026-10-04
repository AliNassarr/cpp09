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

    bool loadDatabase(const std::string& dbPath);
    void evaluate(const std::string& inputPath) const;

private:
    std::map<std::string, double> _rates;

    static std::string trim(const std::string& s);
    static bool isValidDate(const std::string& date);
    static bool parseNumber(const std::string& s, double& value);
    bool findRate(const std::string& date, double& rate) const;
};

#endif
