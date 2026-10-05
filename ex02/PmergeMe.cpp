#include "PmergeMe.hpp"
#include <iostream>
#include <algorithm>
#include <utility>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <sys/time.h>
#include <iomanip>
#include <stdexcept>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) {
    (void)other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& rhs) {
    (void)rhs;
    return *this;
}

PmergeMe::~PmergeMe() {}

std::vector<std::size_t> PmergeMe::getJacobOrderVector(std::size_t n) {
    std::vector<std::size_t> order;
    if (n == 0)
        return order;

    std::vector<std::size_t> j;
    j.push_back(0);
    j.push_back(1);
    while (j.back() < n) {
        j.push_back(j[j.size() - 1] + 2 * j[j.size() - 2]);
    }

    std::vector<bool> added(n + 1, false);
    for (std::size_t i = 1; i < j.size(); ++i) {
        std::size_t val = j[i];
        while (val > j[i - 1] && val <= n) {
            if (!added[val]) {
                order.push_back(val - 1);
                added[val] = true;
            }
            --val;
        }
    }
    for (std::size_t i = 1; i <= n; ++i) {
        if (!added[i])
            order.push_back(i - 1);
    }
    return order;
}

std::deque<std::size_t> PmergeMe::getJacobOrderDeque(std::size_t n) {
    std::deque<std::size_t> order;
    if (n == 0)
        return order;

    std::deque<std::size_t> j;
    j.push_back(0);
    j.push_back(1);
    while (j.back() < n) {
        j.push_back(j[j.size() - 1] + 2 * j[j.size() - 2]);
    }

    std::deque<bool> added(n + 1, false);
    for (std::size_t i = 1; i < j.size(); ++i) {
        std::size_t val = j[i];
        while (val > j[i - 1] && val <= n) {
            if (!added[val]) {
                order.push_back(val - 1);
                added[val] = true;
            }
            --val;
        }
    }
    for (std::size_t i = 1; i <= n; ++i) {
        if (!added[i])
            order.push_back(i - 1);
    }
    return order;
}

void PmergeMe::sortVector(std::vector<int>& arr) {
    std::size_t n = arr.size();
    if (n <= 1)
        return;

    bool hasStraggler = (n % 2 != 0);
    int straggler = 0;
    if (hasStraggler) {
        straggler = arr.back();
        arr.pop_back();
    }

    // 1. Group adjacent elements into pairs of (larger, smaller)
    std::vector<std::pair<int, int> > pairs;
    for (std::size_t i = 0; i < arr.size(); i += 2) {
        if (arr[i] > arr[i + 1])
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }

    // 2. Extract larger elements to form mainChain and sort recursively
    std::vector<int> mainChain;
    for (std::size_t i = 0; i < pairs.size(); ++i)
        mainChain.push_back(pairs[i].first);

    sortVector(mainChain);

    // 3. Align pending elements with the sorted mainChain
    std::vector<int> pend;
    std::vector<bool> used(pairs.size(), false);
    for (std::size_t i = 0; i < mainChain.size(); ++i) {
        for (std::size_t j = 0; j < pairs.size(); ++j) {
            if (!used[j] && pairs[j].first == mainChain[i]) {
                pend.push_back(pairs[j].second);
                used[j] = true;
                break;
            }
        }
    }

    // 4. Insert first pend element into mainChain at index 0 (0 comparisons)
    mainChain.insert(mainChain.begin(), pend[0]);

    // 5. Insert remaining pend elements in Jacobsthal order using binary search (lower_bound)
    if (pend.size() > 1) {
        std::vector<std::size_t> order = getJacobOrderVector(pend.size());
        for (std::size_t i = 0; i < order.size(); ++i) {
            std::size_t idx = order[i];
            if (idx == 0)
                continue;

            int val = pend[idx];
            int partner = 0;
            for (std::size_t j = 0; j < pairs.size(); ++j) {
                if (pairs[j].second == val) {
                    partner = pairs[j].first;
                    break;
                }
            }

            std::vector<int>::iterator itPartner = std::find(mainChain.begin(), mainChain.end(), partner);
            std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), itPartner, val);
            mainChain.insert(pos, val);
        }
    }

    // 6. Insert odd straggler if present
    if (hasStraggler) {
        std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
        mainChain.insert(pos, straggler);
    }

    arr = mainChain;
}

void PmergeMe::sortDeque(std::deque<int>& arr) {
    std::size_t n = arr.size();
    if (n <= 1)
        return;

    bool hasStraggler = (n % 2 != 0);
    int straggler = 0;
    if (hasStraggler) {
        straggler = arr.back();
        arr.pop_back();
    }

    // 1. Group adjacent elements into pairs of (larger, smaller)
    std::deque<std::pair<int, int> > pairs;
    for (std::size_t i = 0; i < arr.size(); i += 2) {
        if (arr[i] > arr[i + 1])
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }

    // 2. Extract larger elements to form mainChain and sort recursively
    std::deque<int> mainChain;
    for (std::size_t i = 0; i < pairs.size(); ++i)
        mainChain.push_back(pairs[i].first);

    sortDeque(mainChain);

    // 3. Align pending elements with the sorted mainChain
    std::deque<int> pend;
    std::deque<bool> used(pairs.size(), false);
    for (std::size_t i = 0; i < mainChain.size(); ++i) {
        for (std::size_t j = 0; j < pairs.size(); ++j) {
            if (!used[j] && pairs[j].first == mainChain[i]) {
                pend.push_back(pairs[j].second);
                used[j] = true;
                break;
            }
        }
    }

    // 4. Insert first pend element into mainChain at index 0 (0 comparisons)
    mainChain.push_front(pend[0]);

    // 5. Insert remaining pend elements in Jacobsthal order using binary search (lower_bound)
    if (pend.size() > 1) {
        std::deque<std::size_t> order = getJacobOrderDeque(pend.size());
        for (std::size_t i = 0; i < order.size(); ++i) {
            std::size_t idx = order[i];
            if (idx == 0)
                continue;

            int val = pend[idx];
            int partner = 0;
            for (std::size_t j = 0; j < pairs.size(); ++j) {
                if (pairs[j].second == val) {
                    partner = pairs[j].first;
                    break;
                }
            }

            std::deque<int>::iterator itPartner = std::find(mainChain.begin(), mainChain.end(), partner);
            std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), itPartner, val);
            mainChain.insert(pos, val);
        }
    }

    // 6. Insert odd straggler if present
    if (hasStraggler) {
        std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
        mainChain.insert(pos, straggler);
    }

    arr = mainChain;
}

void PmergeMe::run(int argc, char** argv) {
    std::vector<int> numbers;
    numbers.reserve(argc - 1);

    for (int i = 1; i < argc; ++i) {
        std::string s = argv[i];
        if (s.empty())
            throw std::runtime_error("Error");

        std::size_t start = 0;
        if (s[0] == '+') {
            start = 1;
            if (s.length() == 1)
                throw std::runtime_error("Error");
        }

        for (std::size_t j = start; j < s.length(); ++j) {
            if (!std::isdigit(static_cast<unsigned char>(s[j])))
                throw std::runtime_error("Error");
        }

        char* endptr = NULL;
        errno = 0;
        long val = std::strtol(s.c_str(), &endptr, 10);
        if (errno == ERANGE || *endptr != '\0' || val < 0 || val > INT_MAX)
            throw std::runtime_error("Error");

        int num = static_cast<int>(val);
        for (std::size_t k = 0; k < numbers.size(); ++k) {
            if (numbers[k] == num)
                throw std::runtime_error("Error");
        }
        numbers.push_back(num);
    }

    std::cout << "Before: ";
    for (std::size_t i = 0; i < numbers.size(); ++i) {
        if (i > 0)
            std::cout << " ";
        std::cout << numbers[i];
    }
    std::cout << std::endl;

    std::vector<int> vecData = numbers;
    std::deque<int> deqData(numbers.begin(), numbers.end());

    struct timeval start, end;

    gettimeofday(&start, NULL);
    sortVector(vecData);
    gettimeofday(&end, NULL);
    double vecTime = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);
    sortDeque(deqData);
    gettimeofday(&end, NULL);
    double deqTime = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

    std::cout << "After:  ";
    for (std::size_t i = 0; i < vecData.size(); ++i) {
        if (i > 0)
            std::cout << " ";
        std::cout << vecData[i];
    }
    std::cout << std::endl;

    std::cout << "Time to process a range of " << numbers.size()
              << " elements with std::vector : " << std::fixed << std::setprecision(5)
              << vecTime << " us" << std::endl;
    std::cout << "Time to process a range of " << numbers.size()
              << " elements with std::deque  : " << std::fixed << std::setprecision(5)
              << deqTime << " us" << std::endl;
}
