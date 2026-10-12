#include "PmergeMe.hpp"
#include <algorithm>
#include <utility>

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other) { (void)other; }
PmergeMe& PmergeMe::operator=(const PmergeMe& rhs) { (void)rhs; return *this; }
PmergeMe::~PmergeMe() {}

std::vector<size_t> PmergeMe::getJacobOrderVector(size_t len) {
    std::vector<size_t> order;
    if (len <= 1) return order;

    std::vector<size_t> j;
    j.push_back(0);
    j.push_back(1);
    while (j.back() < len) {
        j.push_back(j[j.size() - 1] + 2 * j[j.size() - 2]);
    }

    for (size_t i = 3; i < j.size(); ++i) {
        if (j[i - 1] >= len) break;
        size_t start = std::min(j[i], len) - 1;
        size_t end = j[i - 1];
        for (size_t k = start; k >= end; --k) {
            order.push_back(k);
        }
    }
    return order;
}

void PmergeMe::sortVector(std::vector<int>& arr) {
    if (arr.size() < 2) return;

    // Step 1: Save straggler (if odd) and pair adjacent numbers into (winner, loser)
    bool hasStraggler = (arr.size() % 2 != 0);
    int straggler = hasStraggler ? arr.back() : 0;
    if (hasStraggler) arr.pop_back();

    std::vector<std::pair<int, int> > pairs;
    std::vector<int> winners;
    for (size_t i = 0; i < arr.size(); i += 2) {
        int winner = std::max(arr[i], arr[i + 1]);
        int loser = std::min(arr[i], arr[i + 1]);
        pairs.push_back(std::make_pair(winner, loser));
        winners.push_back(winner);
    }

    // Step 2: Recursively sort winners and match each sorted winner with its corresponding loser
    sortVector(winners);

    std::vector<int> losers;
    for (size_t i = 0; i < winners.size(); ++i) {
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (pairs[j].first == winners[i]) {
                losers.push_back(pairs[j].second);
                break;
            }
        }
    }

    // Step 3: Build mainChain by placing losers[0] at the very front for free, followed by all sorted winners
    std::vector<int> mainChain;
    mainChain.push_back(losers[0]);
    for (size_t i = 0; i < winners.size(); ++i) {
        mainChain.push_back(winners[i]);
    }

    // Step 4: Insert remaining losers (and straggler if odd) into mainChain using binary search in Jacobsthal order, bounding up to each loser's paired winner
    std::vector<size_t> jacobOrder = getJacobOrderVector(losers.size());
    for (size_t i = 0; i < jacobOrder.size(); ++i) {
        size_t idx = jacobOrder[i];
        std::vector<int>::iterator bound = std::find(mainChain.begin(), mainChain.end(), winners[idx]);
        std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), bound, losers[idx]);
        mainChain.insert(pos, losers[idx]);
    }

    if (hasStraggler) {
        std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
        mainChain.insert(pos, straggler);
    }

    arr = mainChain;
}

std::deque<size_t> PmergeMe::getJacobOrderDeque(size_t len) {
    std::deque<size_t> order;
    if (len <= 1) return order;

    std::deque<size_t> j;
    j.push_back(0);
    j.push_back(1);
    while (j.back() < len) {
        j.push_back(j[j.size() - 1] + 2 * j[j.size() - 2]);
    }

    for (size_t i = 3; i < j.size(); ++i) {
        if (j[i - 1] >= len) break;
        size_t start = std::min(j[i], len) - 1;
        size_t end = j[i - 1];
        for (size_t k = start; k >= end; --k) {
            order.push_back(k);
        }
    }
    return order;
}

void PmergeMe::sortDeque(std::deque<int>& arr) {
    if (arr.size() < 2) return;

    // Step 1: Save straggler (if odd) and pair adjacent numbers into (winner, loser)
    bool hasStraggler = (arr.size() % 2 != 0);
    int straggler = hasStraggler ? arr.back() : 0;
    if (hasStraggler) arr.pop_back();

    std::deque<std::pair<int, int> > pairs;
    std::deque<int> winners;
    for (size_t i = 0; i < arr.size(); i += 2) {
        int winner = std::max(arr[i], arr[i + 1]);
        int loser = std::min(arr[i], arr[i + 1]);
        pairs.push_back(std::make_pair(winner, loser));
        winners.push_back(winner);
    }

    // Step 2: Recursively sort winners and match each sorted winner with its corresponding loser
    sortDeque(winners);

    std::deque<int> losers;
    for (size_t i = 0; i < winners.size(); ++i) {
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (pairs[j].first == winners[i]) {
                losers.push_back(pairs[j].second);
                break;
            }
        }
    }

    // Step 3: Build mainChain by placing losers[0] at the very front for free, followed by all sorted winners
    std::deque<int> mainChain;
    mainChain.push_back(losers[0]);
    for (size_t i = 0; i < winners.size(); ++i) {
        mainChain.push_back(winners[i]);
    }

    // Step 4: Insert remaining losers (and straggler if odd) into mainChain using binary search in Jacobsthal order, bounding up to each loser's paired winner
    std::deque<size_t> jacobOrder = getJacobOrderDeque(losers.size());
    for (size_t i = 0; i < jacobOrder.size(); ++i) {
        size_t idx = jacobOrder[i];
        std::deque<int>::iterator bound = std::find(mainChain.begin(), mainChain.end(), winners[idx]);
        std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), bound, losers[idx]);
        mainChain.insert(pos, losers[idx]);
    }

    if (hasStraggler) {
        std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
        mainChain.insert(pos, straggler);
    }

    arr = mainChain;
}
