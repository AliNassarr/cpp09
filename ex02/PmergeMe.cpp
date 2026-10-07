#include "PmergeMe.hpp"
#include <algorithm>
#include <utility>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) {
    (void)other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& rhs) {
    (void)rhs;
    return *this;
}

PmergeMe::~PmergeMe() {}

std::vector<size_t> PmergeMe::getJacobOrderVector(size_t len) {
    std::vector<size_t> jacob;
    jacob.push_back(0);
    jacob.push_back(1);

    size_t i = 2;
    while (jacob.back() < len) {
        jacob.push_back(jacob[i - 1] + 2 * jacob[i - 2]);
        i++;
    }

    std::vector<size_t> order;
    for (size_t j = 2; j < jacob.size(); j++) {
        size_t start = std::min(jacob[j], len);
        size_t end = jacob[j - 1];
        for (size_t k = start; k > end; k--) {
            order.push_back(k);
        }
    }
    return order;
}

void PmergeMe::sortVector(std::vector<int>& arr) {
    if (arr.size() < 2) return;

    // 1. Separate the odd element if the size is odd
    bool hasOdd = (arr.size() % 2 != 0);
    int oddElement = 0;
    if (hasOdd) {
        oddElement = arr.back();
        arr.pop_back();
    }

    // 2. Group elements into pairs and sort each pair (max, min)
    std::vector<std::pair<int, int> > pairs;
    for (size_t i = 0; i < arr.size(); i += 2) {
        int max_val = std::max(arr[i], arr[i + 1]);
        int min_val = std::min(arr[i], arr[i + 1]);
        pairs.push_back(std::make_pair(max_val, min_val));
    }

    // 3. Extract the larger elements and recursively sort them
    std::vector<int> recursiveArr;
    for (size_t i = 0; i < pairs.size(); i++) {
        recursiveArr.push_back(pairs[i].first);
    }
    sortVector(recursiveArr);

    // 4. Build the main chain and the pend elements
    std::vector<int> mainChain;
    std::vector<int> pend;
    for (size_t i = 0; i < recursiveArr.size(); i++) {
        mainChain.push_back(recursiveArr[i]);
        for (size_t j = 0; j < pairs.size(); j++) {
            if (pairs[j].first == recursiveArr[i]) {
                pend.push_back(pairs[j].second);
                break;
            }
        }
    }

    // 5. Insert pend elements using the Jacobsthal sequence
    if (!pend.empty()) {
        mainChain.insert(mainChain.begin(), pend[0]); // pend[0] doesn't need comparison
        
        std::vector<size_t> jacobOrder = getJacobOrderVector(pend.size());
        for (size_t i = 0; i < jacobOrder.size(); i++) {
            size_t idx = jacobOrder[i] - 1;
            if (idx == 0) continue; // Already inserted

            std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pend[idx]);
            mainChain.insert(pos, pend[idx]);
        }
    }

    // 6. Insert the odd element if we had one
    if (hasOdd) {
        std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), oddElement);
        mainChain.insert(pos, oddElement);
    }

    arr = mainChain;
}

std::deque<size_t> PmergeMe::getJacobOrderDeque(size_t len) {
    std::deque<size_t> jacob;
    jacob.push_back(0);
    jacob.push_back(1);

    size_t i = 2;
    while (jacob.back() < len) {
        jacob.push_back(jacob[i - 1] + 2 * jacob[i - 2]);
        i++;
    }

    std::deque<size_t> order;
    for (size_t j = 2; j < jacob.size(); j++) {
        size_t start = std::min(jacob[j], len);
        size_t end = jacob[j - 1];
        for (size_t k = start; k > end; k--) {
            order.push_back(k);
        }
    }
    return order;
}

void PmergeMe::sortDeque(std::deque<int>& arr) {
    if (arr.size() < 2) return;

    bool hasOdd = (arr.size() % 2 != 0);
    int oddElement = 0;
    if (hasOdd) {
        oddElement = arr.back();
        arr.pop_back();
    }

    std::deque<std::pair<int, int> > pairs;
    for (size_t i = 0; i < arr.size(); i += 2) {
        int max_val = std::max(arr[i], arr[i + 1]);
        int min_val = std::min(arr[i], arr[i + 1]);
        pairs.push_back(std::make_pair(max_val, min_val));
    }

    std::deque<int> recursiveArr;
    for (size_t i = 0; i < pairs.size(); i++) {
        recursiveArr.push_back(pairs[i].first);
    }
    sortDeque(recursiveArr);

    std::deque<int> mainChain;
    std::deque<int> pend;
    for (size_t i = 0; i < recursiveArr.size(); i++) {
        mainChain.push_back(recursiveArr[i]);
        for (size_t j = 0; j < pairs.size(); j++) {
            if (pairs[j].first == recursiveArr[i]) {
                pend.push_back(pairs[j].second);
                break;
            }
        }
    }

    if (!pend.empty()) {
        mainChain.insert(mainChain.begin(), pend[0]);
        
        std::deque<size_t> jacobOrder = getJacobOrderDeque(pend.size());
        for (size_t i = 0; i < jacobOrder.size(); i++) {
            size_t idx = jacobOrder[i] - 1;
            if (idx == 0) continue;

            std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pend[idx]);
            mainChain.insert(pos, pend[idx]);
        }
    }

    if (hasOdd) {
        std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), oddElement);
        mainChain.insert(pos, oddElement);
    }

    arr = mainChain;
}
