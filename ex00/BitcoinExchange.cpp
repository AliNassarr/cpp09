#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) {
    _db = other._db;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& rhs) {
    if (this != &rhs) {
        _db = rhs._db;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

std::string BitcoinExchange::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

bool BitcoinExchange::isValidDate(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    // Check if remaining characters are digits
    for (size_t i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(date[i])) return false;
    }

    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());

    // Basic bounds checking
    if (year < 1000 || year > 9999) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    // Month specific bounds checking
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeap && day > 29) return false;
        if (!isLeap && day > 28) return false;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    }
    return true;
}

bool BitcoinExchange::parseValue(const std::string& str, double& value) {
    if (str.empty() || str[0] == '.' || str[str.length() - 1] == '.') return false;
    if (str[0] == '+') return false;

    int dots = 0;
    size_t start = (str[0] == '-') ? 1 : 0;
    
    for (size_t i = start; i < str.length(); ++i) {
        if (str[i] == '.') {
            dots++;
            if (dots > 1) return false;
        } else if (!std::isdigit(str[i])) {
            return false;
        }
    }

    char* endptr = NULL;
    value = std::strtod(str.c_str(), &endptr);
    if (endptr == NULL || *endptr != '\0') return false;

    return true;
}

bool BitcoinExchange::loadDatabase(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open database file." << std::endl;
        return false;
    }

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t comma = line.find(',');
        if (comma == std::string::npos) continue;

        std::string date = trim(line.substr(0, comma));
        std::string rateStr = trim(line.substr(comma + 1));

        if (!isValidDate(date)) continue;

        double rate = 0.0;
        if (parseValue(rateStr, rate)) {
            _db[date] = rate;
        }
    }

    return !_db.empty();
}

void BitcoinExchange::processInput(const std::string& filename) const {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);
    if (trim(line) != "date | value") {
        std::cerr << "Error: invalid input file header." << std::endl;
        return;
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t pipe = line.find('|');
        if (pipe == std::string::npos) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        std::string date = trim(line.substr(0, pipe));
        std::string valStr = trim(line.substr(pipe + 1));

        if (!isValidDate(date)) {
            std::cerr << "Error: bad input => " << (date.empty() ? line : date) << std::endl;
            continue;
        }

        double value = 0.0;
        if (!parseValue(valStr, value)) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (value < 0.0) {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }

        if (value > 1000.0) {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }

        std::map<std::string, double>::const_iterator it = _db.upper_bound(date);
        if (it == _db.begin()) {
            std::cerr << "Error: no data available for date " << date << std::endl;
            continue;
        }
        --it; // Step backwards to find the closest earlier (or exact) date

        std::cout << date << " => " << value << " = " << (value * it->second) << std::endl;
    }
}
