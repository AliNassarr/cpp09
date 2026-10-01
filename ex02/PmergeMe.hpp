#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <cstddef>

class PmergeMe
{
public:
	PmergeMe();
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& rhs);
	~PmergeMe();

	void sortVector(std::vector<int>& container);
	void sortDeque(std::deque<int>& container);

private:
	static std::vector<std::size_t> _buildJacobSequence(std::size_t count);

	static void _binaryInsertVector(std::vector<int>& dest, int value, std::size_t limit);
	static void _binaryInsertDeque(std::deque<int>& dest, int value, std::size_t limit);
};

#endif
