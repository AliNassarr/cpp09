#include "PmergeMe.hpp"
#include <algorithm>
#include <utility>

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other) { (void)other; }
PmergeMe& PmergeMe::operator=(const PmergeMe& rhs) { (void)rhs; return *this; }
PmergeMe::~PmergeMe() {}

std::vector<size_t> PmergeMe::getJacobOrderVector(size_t len) {
    std::vector<size_t> j;
    j.push_back(0);
    j.push_back(1);

    while (j.back() < len) {
        j.push_back(j[j.size() - 1] + 2 * j[j.size() - 2]);
    }

    std::vector<size_t> order;
    for (size_t i = 2; i < j.size(); i++) {
        for (size_t k = std::min(j[i], len); k > j[i - 1]; k--) {
            order.push_back(k);
        }
    }
    return order;
}

void PmergeMe::sortVector(std::vector<int>& arr) {
    if (arr.size() < 2) return;

    bool hasOdd = (arr.size() % 2 != 0);
    int oddElement = hasOdd ? arr.back() : 0;
    if (hasOdd) arr.pop_back();

    std::vector<std::pair<int, int> > pairs;
    std::vector<int> recArr;
    for (size_t i = 0; i < arr.size(); i += 2) {
        pairs.push_back(std::make_pair(std::max(arr[i], arr[i + 1]), std::min(arr[i], arr[i + 1])));
        recArr.push_back(pairs.back().first);
    }

    sortVector(recArr);

    std::vector<int> mainChain;
    std::vector<int> pend;
    for (size_t i = 0; i < recArr.size(); i++) {
        mainChain.push_back(recArr[i]);
        for (size_t j = 0; j < pairs.size(); j++) {
            if (pairs[j].first == recArr[i]) {
                pend.push_back(pairs[j].second);
                break;
            }
        }
    }

    if (!pend.empty()) {
        mainChain.insert(mainChain.begin(), pend[0]);
        std::vector<size_t> jacobOrder = getJacobOrderVector(pend.size());
        for (size_t i = 0; i < jacobOrder.size(); i++) {
            size_t idx = jacobOrder[i] - 1;
            if (idx == 0) continue;

            std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pend[idx]);
            mainChain.insert(pos, pend[idx]);
        }
    }

    if (hasOdd) {
        std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), oddElement);
        mainChain.insert(pos, oddElement);
    }

    arr = mainChain;
}

std::deque<size_t> PmergeMe::getJacobOrderDeque(size_t len) {
    std::deque<size_t> j;
    j.push_back(0);
    j.push_back(1);

    while (j.back() < len) {
        j.push_back(j[j.size() - 1] + 2 * j[j.size() - 2]);
    }

    std::deque<size_t> order;
    for (size_t i = 2; i < j.size(); i++) {
        for (size_t k = std::min(j[i], len); k > j[i - 1]; k--) {
            order.push_back(k);
        }
    }
    return order;
}

void PmergeMe::sortDeque(std::deque<int>& arr) {
    if (arr.size() < 2) return;

    bool hasOdd = (arr.size() % 2 != 0);
    int oddElement = hasOdd ? arr.back() : 0;
    if (hasOdd) arr.pop_back();

    std::deque<std::pair<int, int> > pairs;
    std::deque<int> recArr;
    for (size_t i = 0; i < arr.size(); i += 2) {
        pairs.push_back(std::make_pair(std::max(arr[i], arr[i + 1]), std::min(arr[i], arr[i + 1])));
        recArr.push_back(pairs.back().first);
    }

    sortDeque(recArr);

    std::deque<int> mainChain;
    std::deque<int> pend;
    for (size_t i = 0; i < recArr.size(); i++) {
        mainChain.push_back(recArr[i]);
        for (size_t j = 0; j < pairs.size(); j++) {
            if (pairs[j].first == recArr[i]) {
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
