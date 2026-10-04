#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <climits>
#include <cstdlib>
#include <cctype>
#include <sys/time.h>
#include <algorithm>
#include <stdexcept>

#include <cerrno>

PmergeMe::PmergeMe()
    : _vecComparisons(0), _deqComparisons(0), _vecTimeUs(0.0), _deqTimeUs(0.0) {}

PmergeMe::PmergeMe(const PmergeMe& other)
    : _original(other._original),
      _vectorSorted(other._vectorSorted),
      _dequeSorted(other._dequeSorted),
      _vecComparisons(other._vecComparisons),
      _deqComparisons(other._deqComparisons),
      _vecTimeUs(other._vecTimeUs),
      _deqTimeUs(other._deqTimeUs) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& rhs) {
    if (this != &rhs) {
        _original = rhs._original;
        _vectorSorted = rhs._vectorSorted;
        _dequeSorted = rhs._dequeSorted;
        _vecComparisons = rhs._vecComparisons;
        _deqComparisons = rhs._deqComparisons;
        _vecTimeUs = rhs._vecTimeUs;
        _deqTimeUs = rhs._deqTimeUs;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

double PmergeMe::getMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return static_cast<double>(tv.tv_sec) * 1000000.0 + static_cast<double>(tv.tv_usec);
}

void PmergeMe::parseInput(int argc, char** argv, std::vector<int>& dest) {
    dest.clear();
    dest.reserve(argc - 1);

    for (int i = 1; i < argc; ++i) {
        std::string token = argv[i];
        if (token.empty())
            throw std::runtime_error("Error");

        std::size_t start = 0;
        if (token[0] == '+') {
            start = 1;
            if (token.length() == 1)
                throw std::runtime_error("Error");
        }

        for (std::size_t j = start; j < token.length(); ++j) {
            if (!std::isdigit(static_cast<unsigned char>(token[j])))
                throw std::runtime_error("Error");
        }

        char* endptr = NULL;
        errno = 0;
        long val = std::strtol(token.c_str(), &endptr, 10);
        if (errno == ERANGE || *endptr != '\0' || val < 0 || val > INT_MAX)
            throw std::runtime_error("Error");

        int intVal = static_cast<int>(val);
        for (std::size_t k = 0; k < dest.size(); ++k) {
            if (dest[k] == intVal)
                throw std::runtime_error("Error");
        }

        dest.push_back(intVal);
    }
}

template <typename Container>
typename ContainerTraits<Container>::IndexContainer PmergeMe::generateJacobsthal(std::size_t size) {
    typename ContainerTraits<Container>::IndexContainer seq;
    if (size == 0)
        return seq;

    seq.push_back(0);
    if (size == 1)
        return seq;

    seq.push_back(1);
    while (seq.back() < size) {
        std::size_t nextVal = seq[seq.size() - 1] + 2 * seq[seq.size() - 2];
        seq.push_back(nextVal);
    }
    return seq;
}

template <typename Container>
typename ContainerTraits<Container>::IndexContainer PmergeMe::buildInsertionOrder(
    const typename ContainerTraits<Container>::IndexContainer& jacobSeq,
    std::size_t pendSize) {
    typename ContainerTraits<Container>::IndexContainer order;
    typename ContainerTraits<Container>::BoolContainer visited(pendSize + 1, false);

    for (std::size_t i = 0; i < jacobSeq.size(); ++i) {
        std::size_t idx = jacobSeq[i];
        while (idx > 1 && idx <= pendSize) {
            if (!visited[idx]) {
                order.push_back(idx);
                visited[idx] = true;
            }
            --idx;
        }
    }

    for (std::size_t idx = 2; idx <= pendSize; ++idx) {
        if (!visited[idx]) {
            order.push_back(idx);
            visited[idx] = true;
        }
    }
    return order;
}

template <typename Container>
std::size_t PmergeMe::binarySearchPosition(
    const Container& chain,
    int target,
    std::size_t upperBound,
    std::size_t& comparisons) {
    if (chain.empty())
        return 0;

    int low = 0;
    int high = static_cast<int>(upperBound);
    if (high >= static_cast<int>(chain.size()))
        high = static_cast<int>(chain.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        comparisons++;
        if (chain[mid] >= target)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return static_cast<std::size_t>(low);
}

template <typename Container>
void PmergeMe::insertPending(Container& mainChain, const Container& pendChain, std::size_t& comparisons) {
    if (pendChain.empty())
        return;

    typename ContainerTraits<Container>::IndexContainer jacobSeq =
        generateJacobsthal<Container>(pendChain.size());
    typename ContainerTraits<Container>::IndexContainer order =
        buildInsertionOrder<Container>(jacobSeq, pendChain.size());

    std::size_t searchLimit = 3;
    mainChain.insert(mainChain.begin(), pendChain[0]);

    for (std::size_t i = 0; i < order.size(); ++i) {
        if (i > 0 && order[i] > order[i - 1]) {
            if (searchLimit <= mainChain.size() / 2)
                searchLimit = 2 * searchLimit + 1;
            else
                searchLimit = mainChain.size();
        }

        if (order[i] <= pendChain.size() && order[i] != 1) {
            std::size_t pendIdx = order[i] - 1;
            int targetVal = pendChain[pendIdx];
            std::size_t maxSearchIdx = std::min(searchLimit - 1, mainChain.size());
            std::size_t pos = binarySearchPosition(mainChain, targetVal, maxSearchIdx, comparisons);
            mainChain.insert(mainChain.begin() + pos, targetVal);
        }
    }
}

template <typename Container>
Container PmergeMe::recursiveMergeInsert(Container& container, std::size_t& comparisons) {
    if (container.size() <= 1)
        return container;

    if (container.size() == 2) {
        Container result = container;
        comparisons++;
        if (result[0] > result[1])
            std::swap(result[0], result[1]);
        return result;
    }

    bool isOdd = (container.size() % 2 == 1);
    Container mainChain;
    Container pendChain;

    for (std::size_t i = 0; i < container.size() - (isOdd ? 1 : 0); i += 2) {
        comparisons++;
        if (container[i] > container[i + 1]) {
            mainChain.push_back(container[i]);
            pendChain.push_back(container[i + 1]);
        } else {
            pendChain.push_back(container[i]);
            mainChain.push_back(container[i + 1]);
        }
    }

    if (isOdd)
        pendChain.push_back(container.back());

    Container sortedMain = recursiveMergeInsert(mainChain, comparisons);

    Container alignedPend;
    for (std::size_t i = 0; i < sortedMain.size(); ++i) {
        for (std::size_t j = 0; j < mainChain.size(); ++j) {
            if (mainChain[j] == sortedMain[i]) {
                alignedPend.push_back(pendChain[j]);
                break;
            }
        }
    }

    if (isOdd)
        alignedPend.push_back(pendChain.back());

    insertPending(sortedMain, alignedPend, comparisons);
    return sortedMain;
}

template <typename Container>
void PmergeMe::fordJohnsonSort(Container& container, std::size_t& comparisons) {
    comparisons = 0;
    if (container.size() <= 1)
        return;
    container = recursiveMergeInsert(container, comparisons);
}

void PmergeMe::printSequence(const std::vector<int>& seq, const std::string& prefix) const {
    std::cout << prefix;
    for (std::size_t i = 0; i < seq.size(); ++i) {
        if (i > 0)
            std::cout << " ";
        std::cout << seq[i];
    }
    std::cout << std::endl;
}

void PmergeMe::printBenchmark() const {
    std::cout << "Time to process a range of " << _original.size()
              << " elements with std::vector : " << std::fixed << std::setprecision(5)
              << _vecTimeUs << " us" << std::endl;
    std::cout << "Time to process a range of " << _original.size()
              << " elements with std::deque  : " << std::fixed << std::setprecision(5)
              << _deqTimeUs << " us" << std::endl;
    std::cout << "Number of comparisons with std::vector : " << _vecComparisons << std::endl;
    std::cout << "Number of comparisons with std::deque  : " << _deqComparisons << std::endl;
}

void PmergeMe::run(int argc, char** argv) {
    parseInput(argc, argv, _original);

    printSequence(_original, "Before:\t\t");

    _vectorSorted = _original;
    double startVec = getMicroseconds();
    fordJohnsonSort(_vectorSorted, _vecComparisons);
    double endVec = getMicroseconds();
    _vecTimeUs = endVec - startVec;

    _dequeSorted.assign(_original.begin(), _original.end());
    double startDeq = getMicroseconds();
    fordJohnsonSort(_dequeSorted, _deqComparisons);
    double endDeq = getMicroseconds();
    _deqTimeUs = endDeq - startDeq;

    printSequence(_vectorSorted, "After:\t\t");
    printBenchmark();
}
