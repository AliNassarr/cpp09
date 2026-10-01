#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <string>
#include <map>

class BitcoinExchange
{
public:
	BitcoinExchange();
	explicit BitcoinExchange(const std::string& databasePath);
	BitcoinExchange(const BitcoinExchange& other);
	BitcoinExchange& operator=(const BitcoinExchange& rhs);
	~BitcoinExchange();

	bool loadDatabase(const std::string& databasePath);
	void processInput(const std::string& inputPath) const;

private:
	std::map<std::string, double> _rates;

	static bool _isValidDate(const std::string& date);
	static bool _isValidValue(const std::string& valueStr, double& value);
	double _getExchangeRate(const std::string& date) const;
};

#endif
