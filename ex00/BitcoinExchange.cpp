#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const std::string& databasePath)
{
	loadDatabase(databasePath);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
	: _rates(other._rates)
{
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& rhs)
{
	if (this != &rhs)
	{
		_rates = rhs._rates;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

bool BitcoinExchange::loadDatabase(const std::string& databasePath)
{
	std::ifstream dbFile(databasePath.c_str());
	if (!dbFile.is_open())
		return false;

	std::string line;
	if (!std::getline(dbFile, line))
		return false; // Empty file

	while (std::getline(dbFile, line))
	{
		if (line.empty())
			continue;

		std::string::size_type commaPos = line.find(',');
		if (commaPos == std::string::npos)
			continue;

		std::string date = line.substr(0, commaPos);
		std::string rateStr = line.substr(commaPos + 1);

		char* endPtr = NULL;
		double rate = std::strtod(rateStr.c_str(), &endPtr);
		if (endPtr != rateStr.c_str())
			_rates[date] = rate;
	}
	dbFile.close();
	return true;
}

bool BitcoinExchange::_isValidDate(const std::string& date)
{
	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return false;

	for (std::size_t i = 0; i < date.length(); ++i)
	{
		if (i == 4 || i == 7)
			continue;
		if (date[i] < '0' || date[i] > '9')
			return false;
	}

	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());

	if (year < 2009 || month < 1 || month > 12 || day < 1 || day > 31)
		return false;

	const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int maxDays = daysInMonth[month - 1];

	if (month == 2)
	{
		bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
		if (isLeap)
			maxDays = 29;
	}

	return day <= maxDays;
}

bool BitcoinExchange::_isValidValue(const std::string& valueStr, double& value)
{
	char* endPtr = NULL;
	value = std::strtod(valueStr.c_str(), &endPtr);

	if (endPtr == valueStr.c_str() || *endPtr != '\0')
		return false;
	return true;
}

double BitcoinExchange::_getExchangeRate(const std::string& date) const
{
	std::map<std::string, double>::const_iterator it = _rates.upper_bound(date);
	if (it == _rates.begin())
		return -1.0;
	--it;
	return it->second;
}

void BitcoinExchange::processInput(const std::string& inputPath) const
{
	std::ifstream inFile(inputPath.c_str());
	if (!inFile.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}

	std::string line;
	if (std::getline(inFile, line))
	{
		// Skip header line if present (e.g. "date | value")
		if (line.find("date") == std::string::npos || line.find("value") == std::string::npos)
		{
			// Reset to beginning if first line is data
			inFile.clear();
			inFile.seekg(0, std::ios::beg);
		}
	}

	while (std::getline(inFile, line))
	{
		if (line.empty())
			continue;

		std::string::size_type pipePos = line.find('|');
		if (pipePos == std::string::npos)
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::string date = line.substr(0, pipePos);
		// Trim trailing whitespace from date
		std::string::size_type end = date.find_last_not_of(" \t");
		if (end != std::string::npos)
			date = date.substr(0, end + 1);

		// Trim leading whitespace from date
		std::string::size_type start = date.find_first_not_of(" \t");
		if (start != std::string::npos)
			date = date.substr(start);

		if (!_isValidDate(date))
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::string valStr = line.substr(pipePos + 1);
		// Trim whitespace from valStr
		start = valStr.find_first_not_of(" \t");
		end = valStr.find_last_not_of(" \t");
		if (start != std::string::npos && end != std::string::npos)
			valStr = valStr.substr(start, end - start + 1);
		else
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		double value = 0.0;
		if (!_isValidValue(valStr, value))
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		if (value < 0.0)
		{
			std::cerr << "Error: not a positive number." << std::endl;
			continue;
		}
		if (value > 1000.0)
		{
			std::cerr << "Error: too large a number." << std::endl;
			continue;
		}

		double rate = _getExchangeRate(date);
		if (rate < 0.0)
		{
			std::cerr << "Error: no rate available for " << date << std::endl;
			continue;
		}

		std::cout << date << " => " << value << " = " << (value * rate) << std::endl;
	}
	inFile.close();
}
