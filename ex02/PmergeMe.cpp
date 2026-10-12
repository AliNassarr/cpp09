#include "PmergeMe.hpp"
#include <algorithm>
#include <utility>

size_t PmergeMe::comparisons = 0;

bool PmergeMe::compare(int a, int b) {
    comparisons++;
    return a < b;
}

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other) { (void)other; }
PmergeMe& PmergeMe::operator=(const PmergeMe& rhs) { (void)rhs; return *this; }
PmergeMe::~PmergeMe() {}

void PmergeMe::sortVector(std::vector<int>& arr) {
    if (arr.size() < 2) return;

    // Step 1: Pair adjacent elements into (winner, loser)
    std::vector<std::pair<int, int> > pairs;
    std::vector<int> winners;
    for (size_t i = 0; i + 1 < arr.size(); i += 2) {
        int winner = arr[i], loser = arr[i + 1];
        if (compare(winner, loser))
            std::swap(winner, loser);
        pairs.push_back(std::make_pair(winner, loser));
        winners.push_back(winner);
    }

    // Step 2: Recursively sort winners and match each sorted winner with its loser
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

    // Step 3: Build mainChain with losers[0] at front for free, followed by sorted winners
    std::vector<int> mainChain = winners;
    mainChain.insert(mainChain.begin(), losers[0]);

    // Step 4: Insert remaining losers in Jacobsthal order, bounded by paired winner
    for (size_t jPrev = 1, jCurr = 3; jPrev < losers.size(); ) {
        for (size_t k = std::min(jCurr, losers.size()); k > jPrev; --k) {
            size_t idx = k - 1;
            std::vector<int>::iterator bound = std::find(mainChain.begin(), mainChain.end(), winners[idx]);
            mainChain.insert(std::lower_bound(mainChain.begin(), bound, losers[idx], compare), losers[idx]);
        }
        size_t nextJ = jCurr + 2 * jPrev;
        jPrev = jCurr;
        jCurr = nextJ;
    }

    // Insert odd straggler if present
    if (arr.size() % 2 != 0)
        mainChain.insert(std::lower_bound(mainChain.begin(), mainChain.end(), arr.back(), compare), arr.back());

    arr = mainChain;
}

void PmergeMe::sortDeque(std::deque<int>& arr) {
    if (arr.size() < 2) return;

    // Step 1: Pair adjacent elements into (winner, loser)
    std::deque<std::pair<int, int> > pairs;
    std::deque<int> winners;
    for (size_t i = 0; i + 1 < arr.size(); i += 2) {
        int winner = arr[i], loser = arr[i + 1];
        if (compare(winner, loser))
            std::swap(winner, loser);
        pairs.push_back(std::make_pair(winner, loser));
        winners.push_back(winner);
    }

    // Step 2: Recursively sort winners and match each sorted winner with its loser
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

    // Step 3: Build mainChain with losers[0] at front for free, followed by sorted winners
    std::deque<int> mainChain = winners;
    mainChain.insert(mainChain.begin(), losers[0]);

    // Step 4: Insert remaining losers in Jacobsthal order, bounded by paired winner
    for (size_t jPrev = 1, jCurr = 3; jPrev < losers.size(); ) {
        for (size_t k = std::min(jCurr, losers.size()); k > jPrev; --k) {
            size_t idx = k - 1;
            std::deque<int>::iterator bound = std::find(mainChain.begin(), mainChain.end(), winners[idx]);
            mainChain.insert(std::lower_bound(mainChain.begin(), bound, losers[idx], compare), losers[idx]);
        }
        size_t nextJ = jCurr + 2 * jPrev;
        jPrev = jCurr;
        jCurr = nextJ;
    }

    // Insert odd straggler if present
    if (arr.size() % 2 != 0)
        mainChain.insert(std::lower_bound(mainChain.begin(), mainChain.end(), arr.back(), compare), arr.back());

    arr = mainChain;
}
