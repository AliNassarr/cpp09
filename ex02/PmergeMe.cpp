#include "PmergeMe.hpp"
#include <algorithm>
#include <utility>

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe& other)
{
	(void)other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& rhs)
{
	(void)rhs;
	return *this;
}

PmergeMe::~PmergeMe()
{
}

std::vector<std::size_t> PmergeMe::_buildJacobSequence(std::size_t count)
{
	std::vector<std::size_t> sequence;
	if (count == 0)
		return sequence;

	std::vector<std::size_t> jacob;
	jacob.push_back(0);
	jacob.push_back(1);

	while (jacob.back() < count)
	{
		std::size_t nextVal = jacob.back() + 2 * jacob[jacob.size() - 2];
		jacob.push_back(nextVal);
	}

	std::size_t lastIndex = 1;
	for (std::size_t k = 3; k < jacob.size(); ++k)
	{
		std::size_t current = jacob[k];
		std::size_t top = (current <= count) ? current : count;
		for (std::size_t idx = top; idx > lastIndex; --idx)
			sequence.push_back(idx - 1);
		lastIndex = top;
		if (lastIndex >= count)
			break;
	}

	for (std::size_t idx = count; idx > lastIndex; --idx)
		sequence.push_back(idx - 1);

	return sequence;
}

void PmergeMe::_binaryInsertVector(std::vector<int>& dest, int value, std::size_t limit)
{
	std::size_t endIdx = (limit < dest.size()) ? limit : dest.size();
	std::vector<int>::iterator it = std::lower_bound(dest.begin(), dest.begin() + endIdx, value);
	dest.insert(it, value);
}

void PmergeMe::_binaryInsertDeque(std::deque<int>& dest, int value, std::size_t limit)
{
	std::size_t endIdx = (limit < dest.size()) ? limit : dest.size();
	std::deque<int>::iterator it = std::lower_bound(dest.begin(), dest.begin() + endIdx, value);
	dest.insert(it, value);
}

void PmergeMe::sortVector(std::vector<int>& container)
{
	std::size_t n = container.size();
	if (n <= 1)
		return;

	bool hasStraggler = (n % 2 != 0);
	int straggler = 0;
	if (hasStraggler)
		straggler = container.back();

	std::size_t pairCount = n / 2;
	std::vector<std::pair<int, int> > pairs(pairCount);
	for (std::size_t i = 0; i < pairCount; ++i)
	{
		int first = container[2 * i];
		int second = container[2 * i + 1];
		if (first >= second)
			pairs[i] = std::make_pair(first, second);
		else
			pairs[i] = std::make_pair(second, first);
	}

	std::vector<int> largerElements(pairCount);
	for (std::size_t i = 0; i < pairCount; ++i)
		largerElements[i] = pairs[i].first;

	sortVector(largerElements);

	std::vector<int> mainChain;
	mainChain.reserve(n);

	std::vector<int> pending;
	pending.reserve(pairCount);

	std::vector<bool> used(pairCount, false);
	for (std::size_t i = 0; i < largerElements.size(); ++i)
	{
		for (std::size_t j = 0; j < pairCount; ++j)
		{
			if (!used[j] && pairs[j].first == largerElements[i])
			{
				mainChain.push_back(pairs[j].first);
				pending.push_back(pairs[j].second);
				used[j] = true;
				break;
			}
		}
	}

	mainChain.insert(mainChain.begin(), pending[0]);

	if (pending.size() > 1)
	{
		std::vector<std::size_t> insertOrder = _buildJacobSequence(pending.size());
		for (std::size_t i = 0; i < insertOrder.size(); ++i)
		{
			std::size_t pendIdx = insertOrder[i];
			if (pendIdx == 0)
				continue;
			int valToInsert = pending[pendIdx];
			int partnerVal = largerElements[pendIdx];

			std::vector<int>::iterator partnerIt = std::find(mainChain.begin(), mainChain.end(), partnerVal);
			std::size_t limit = std::distance(mainChain.begin(), partnerIt);

			_binaryInsertVector(mainChain, valToInsert, limit);
		}
	}

	if (hasStraggler)
		_binaryInsertVector(mainChain, straggler, mainChain.size());

	container = mainChain;
}

void PmergeMe::sortDeque(std::deque<int>& container)
{
	std::size_t n = container.size();
	if (n <= 1)
		return;

	bool hasStraggler = (n % 2 != 0);
	int straggler = 0;
	if (hasStraggler)
		straggler = container.back();

	std::size_t pairCount = n / 2;
	std::deque<std::pair<int, int> > pairs(pairCount);
	for (std::size_t i = 0; i < pairCount; ++i)
	{
		int first = container[2 * i];
		int second = container[2 * i + 1];
		if (first >= second)
			pairs[i] = std::make_pair(first, second);
		else
			pairs[i] = std::make_pair(second, first);
	}

	std::deque<int> largerElements(pairCount);
	for (std::size_t i = 0; i < pairCount; ++i)
		largerElements[i] = pairs[i].first;

	sortDeque(largerElements);

	std::deque<int> mainChain;
	std::deque<int> pending;

	std::vector<bool> used(pairCount, false);
	for (std::size_t i = 0; i < largerElements.size(); ++i)
	{
		for (std::size_t j = 0; j < pairCount; ++j)
		{
			if (!used[j] && pairs[j].first == largerElements[i])
			{
				mainChain.push_back(pairs[j].first);
				pending.push_back(pairs[j].second);
				used[j] = true;
				break;
			}
		}
	}

	mainChain.push_front(pending[0]);

	if (pending.size() > 1)
	{
		std::vector<std::size_t> insertOrder = _buildJacobSequence(pending.size());
		for (std::size_t i = 0; i < insertOrder.size(); ++i)
		{
			std::size_t pendIdx = insertOrder[i];
			if (pendIdx == 0)
				continue;
			int valToInsert = pending[pendIdx];
			int partnerVal = largerElements[pendIdx];

			std::deque<int>::iterator partnerIt = std::find(mainChain.begin(), mainChain.end(), partnerVal);
			std::size_t limit = std::distance(mainChain.begin(), partnerIt);

			_binaryInsertDeque(mainChain, valToInsert, limit);
		}
	}

	if (hasStraggler)
		_binaryInsertDeque(mainChain, straggler, mainChain.size());

	container = mainChain;
}
