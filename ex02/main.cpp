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

void printVec(const std::string& msg, const std::vector<int>& v) {
    std::cout << msg;
    for (size_t i = 0; i < v.size(); ++i) std::cout << v[i] << " ";
    std::cout << "\n";
}

int main(int argc, char** argv) {
    if (argc < 2) { std::cerr << "Error\n"; return 1; }

    std::vector<int> vec;
    std::deque<int> deq;

    // Compact but readable parsing and validation
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        size_t start = (arg[0] == '+') ? 1 : 0;
        
        if (arg.length() == start || arg.find_first_not_of("0123456789", start) != std::string::npos) {
            std::cerr << "Error\n"; return 1;
        }
        
        long val = std::strtol(arg.c_str(), NULL, 10);
        if (val > 2147483647) { std::cerr << "Error\n"; return 1; }

        for (size_t j = 0; j < vec.size(); ++j) {
            if (vec[j] == val) { std::cerr << "Error\n"; return 1; }
        }

        vec.push_back(val);
        deq.push_back(val);
    }

    printVec("Before: ", vec);

    // Timing Vector
    double start = getUs();
    PmergeMe::sortVector(vec);
    double timeVec = getUs() - start;

    // Timing Deque
    start = getUs();
    PmergeMe::sortDeque(deq);
    double timeDeq = getUs() - start;

    printVec("After:  ", vec);

    // Final Output
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << vec.size() << " elements with std::vector : " << timeVec << " us\n";
    std::cout << "Time to process a range of " << deq.size() << " elements with std::deque  : " << timeDeq << " us\n";

    return 0;
}
