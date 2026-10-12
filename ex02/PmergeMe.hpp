#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <cstddef>

class PmergeMe {
public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& rhs);
    ~PmergeMe();

    static size_t comparisons;
    static bool compare(int a, int b);

    static void sortVector(std::vector<int>& arr);
    static void sortDeque(std::deque<int>& arr);
};

#endif
