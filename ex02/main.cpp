#include "PmergeMe.hpp"
#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <iomanip>

double getUs() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec * 1000000.0 + t.tv_usec;
}

bool isValidNumber(const std::string& str) {
    size_t start = (str[0] == '+') ? 1 : 0;
    if (str.length() == start) return false;
    return str.find_first_not_of("0123456789", start) == std::string::npos;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Error\n";
        return 1;
    }

    // 1. Validate inputs upfront
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (!isValidNumber(arg)) {
            std::cerr << "Error\n";
            return 1;
        }
        long val = std::strtol(arg.c_str(), NULL, 10);
        if (val > 2147483647) {
            std::cerr << "Error\n";
            return 1;
        }
        for (int j = 1; j < i; ++j) {
            if (std::strtol(argv[j], NULL, 10) == val) {
                std::cerr << "Error\n";
                return 1;
            }
        }
    }

    // 2. Display sequence before sorting
    std::cout << "Before: ";
    for (int i = 1; i < argc; ++i) {
        std::cout << std::strtol(argv[i], NULL, 10) << " ";
    }
    std::cout << "\n";

    // 3. std::vector: measure data management + sort
    std::vector<int> vec;
    PmergeMe::comparisons = 0;
    double startVec = getUs();
    for (int i = 1; i < argc; ++i) {
        vec.push_back(std::strtol(argv[i], NULL, 10));
    }
    PmergeMe::sortVector(vec);
    double timeVec = getUs() - startVec;
    size_t compVec = PmergeMe::comparisons;

    // 4. std::deque: measure data management + sort
    std::deque<int> deq;
    PmergeMe::comparisons = 0;
    double startDeq = getUs();
    for (int i = 1; i < argc; ++i) {
        deq.push_back(std::strtol(argv[i], NULL, 10));
    }
    PmergeMe::sortDeque(deq);
    double timeDeq = getUs() - startDeq;
    size_t compDeq = PmergeMe::comparisons;

    // 5. Display sequence after sorting
    std::cout << "After:  ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << "\n";

    // 6. Display timings and comparisons
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << vec.size() << " elements with std::vector : " << timeVec << " us\n";
    std::cout << "Time to process a range of " << deq.size() << " elements with std::deque  : " << timeDeq << " us\n";
    std::cout << "Comparisons (std::vector): " << compVec << "\n";
    std::cout << "Comparisons (std::deque):  " << compDeq << "\n";

    return 0;
}
