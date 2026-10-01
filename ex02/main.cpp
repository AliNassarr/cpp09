#include "PmergeMe.hpp"
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <cstdlib>
#include <ctime>
#include <climits>

static bool parsePositiveInt(const char* str, int& value)
{
	if (!str || *str == '\0')
		return false;

	char* endPtr = NULL;
	long parsed = std::strtol(str, &endPtr, 10);

	if (*endPtr != '\0')
		return false;
	if (parsed < 0 || parsed > INT_MAX)
		return false;

	value = static_cast<int>(parsed);
	return true;
}

static void printSequence(const std::vector<int>& vec, const std::string& prefix)
{
	std::cout << prefix;
	for (std::size_t i = 0; i < vec.size(); ++i)
	{
		if (i > 0)
			std::cout << " ";
		std::cout << vec[i];
	}
	std::cout << std::endl;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}

	std::vector<int> numbers;
	numbers.reserve(argc - 1);

	for (int i = 1; i < argc; ++i)
	{
		int val = 0;
		if (!parsePositiveInt(argv[i], val))
		{
			std::cerr << "Error" << std::endl;
			return 1;
		}
		numbers.push_back(val);
	}

	std::vector<int> vecData = numbers;
	std::deque<int> deqData(numbers.begin(), numbers.end());

	PmergeMe sorter;

	std::clock_t startVec = std::clock();
	sorter.sortVector(vecData);
	std::clock_t endVec = std::clock();
	double timeVec = static_cast<double>(endVec - startVec) * 1000000.0 / CLOCKS_PER_SEC;

	std::clock_t startDeq = std::clock();
	sorter.sortDeque(deqData);
	std::clock_t endDeq = std::clock();
	double timeDeq = static_cast<double>(endDeq - startDeq) * 1000000.0 / CLOCKS_PER_SEC;

	printSequence(numbers, "Before:  ");
	printSequence(vecData, "After:   ");

	std::cout << "Time to process a range of " << vecData.size()
			  << " elements with std::vector : " << timeVec << " us" << std::endl;
	std::cout << "Time to process a range of " << deqData.size()
			  << " elements with std::deque  : " << timeDeq << " us" << std::endl;

	return 0;
}
