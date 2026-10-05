#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cctype>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _database(other._database) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& rhs) {
    if (this != &rhs) {
        _database = rhs._database;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

std::string BitcoinExchange::trim(const std::string& s) {
    std::string::size_type start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
        return "";
    std::string::size_type end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool BitcoinExchange::isValidDate(const std::string& date) {
    if (date.length() != 10 || date[4] != '-' || date[7] != '-')
        return false;

    for (std::size_t i = 0; i < 10; ++i) {
        if (i == 4 || i == 7)
            continue;
        if (!std::isdigit(static_cast<unsigned char>(date[i])))
            return false;
    }

    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());

    if (year < 1000 || year > 9999 || month < 1 || month > 12 || day < 1 || day > 31)
        return false;

    if (month == 2) {
        bool isLeap = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
        if (day > (isLeap ? 29 : 28))
            return false;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30)
            return false;
    }
    return true;
}

bool BitcoinExchange::parseValue(const std::string& s, double& value) {
    if (s.empty() || s[0] == '.' || s[s.length() - 1] == '.' || s[0] == '+')
        return false;

    int dotCount = 0;
    std::size_t start = (s[0] == '-') ? 1 : 0;
    if (start == s.length())
        return false;

    for (std::size_t i = start; i < s.length(); ++i) {
        if (s[i] == '.') {
            if (++dotCount > 1)
                return false;
        } else if (!std::isdigit(static_cast<unsigned char>(s[i]))) {
            return false;
        }
    }

    char* endptr = NULL;
    value = std::strtod(s.c_str(), &endptr);
    return (endptr != NULL && *endptr == '\0');
}

bool BitcoinExchange::loadDatabase(const std::string& dbPath) {
    std::ifstream file(dbPath.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open database file." << std::endl;
        return false;
    }

    std::string line;
    if (!std::getline(file, line)) {
        file.close();
        return false;
    }

    while (std::getline(file, line)) {
        if (line.empty())
            continue;
        std::string::size_type comma = line.find(',');
        if (comma == std::string::npos)
            continue;

        std::string date = trim(line.substr(0, comma));
        std::string rateStr = trim(line.substr(comma + 1));
        if (!isValidDate(date))
            continue;

        char* endptr = NULL;
        double rate = std::strtod(rateStr.c_str(), &endptr);
        if (endptr != NULL && *endptr == '\0') {
            _database[date] = rate;
        }
    }

    file.close();
    return !_database.empty();
}

void BitcoinExchange::process(const std::string& inputPath) const {
    std::ifstream file(inputPath.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    if (!std::getline(file, line)) {
        std::cerr << "Error: input file is empty." << std::endl;
        file.close();
        return;
    }

    if (trim(line) != "date | value") {
        std::cerr << "Error: invalid input file header." << std::endl;
        file.close();
        return;
    }

    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        std::string::size_type sep = line.find('|');
        if (sep == std::string::npos) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        std::string dateStr = trim(line.substr(0, sep));
        std::string valStr = trim(line.substr(sep + 1));

        if (dateStr.empty() || !isValidDate(dateStr)) {
            std::cerr << "Error: bad input => " << (dateStr.empty() ? line : dateStr) << std::endl;
            continue;
        }

        double val = 0.0;
        if (!parseValue(valStr, val)) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (val < 0.0) {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }

        if (val > 1000.0) {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }

        std::map<std::string, double>::const_iterator it = _database.upper_bound(dateStr);
        if (it == _database.begin()) {
            std::cerr << "Error: no data available for date " << dateStr << std::endl;
            continue;
        }
        --it;

        std::cout << dateStr << " => " << val << " = " << (val * it->second) << std::endl;
    }

    file.close();
}
