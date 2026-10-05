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
    void process(const std::string& inputPath) const;

private:
    std::map<std::string, double> _database;

    static std::string trim(const std::string& s);
    static bool isValidDate(const std::string& date);
    static bool parseValue(const std::string& s, double& value);
};

#endif
