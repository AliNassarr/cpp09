#include "PmergeMe.hpp"
#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <iomanip>

double getTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000.0 + tv.tv_usec;
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

    std::vector<int> vec;
    std::deque<int> deq;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (!isValidNumber(arg)) {
            std::cerr << "Error\n";
            return 1;
        }

        long val = std::strtol(arg.c_str(), NULL, 10);
        if (val < 0 || val > 2147483647) {
            std::cerr << "Error\n";
            return 1;
        }

        for (size_t j = 0; j < vec.size(); ++j) {
            if (vec[j] == val) {
                std::cerr << "Error\n";
                return 1;
            }
        }

        vec.push_back(val);
        deq.push_back(val);
    }

    std::cout << "Before: ";
    for (size_t i = 0; i < vec.size(); ++i) std::cout << vec[i] << " ";
    std::cout << "\n";

    double startVec = getTime();
    PmergeMe::sortVector(vec);
    double timeVec = getTime() - startVec;

    double startDeq = getTime();
    PmergeMe::sortDeque(deq);
    double timeDeq = getTime() - startDeq;

    std::cout << "After:  ";
    for (size_t i = 0; i < vec.size(); ++i) std::cout << vec[i] << " ";
    std::cout << "\n";

    std::cout << "Time to process a range of " << vec.size() << " elements with std::vector : " 
              << std::fixed << std::setprecision(5) << timeVec << " us\n";
    std::cout << "Time to process a range of " << deq.size() << " elements with std::deque  : " 
              << std::fixed << std::setprecision(5) << timeDeq << " us\n";

    return 0;
}
