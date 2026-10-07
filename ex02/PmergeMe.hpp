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

    static void sortVector(std::vector<int>& arr);
    static void sortDeque(std::deque<int>& arr);

private:
    static std::vector<size_t> getJacobOrderVector(size_t len);
    static std::deque<size_t> getJacobOrderDeque(size_t len);
};

#endif
