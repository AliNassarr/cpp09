#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <cstddef>

class PmergeMe {
public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& rhs);
    ~PmergeMe();

    void run(int argc, char** argv);

    static void sortVector(std::vector<int>& arr);
    static void sortDeque(std::deque<int>& arr);

private:
    static std::vector<std::size_t> getJacobOrderVector(std::size_t n);
    static std::deque<std::size_t>  getJacobOrderDeque(std::size_t n);
};

#endif
