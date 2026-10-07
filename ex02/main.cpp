#include "PmergeMe.hpp"
#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <iomanip>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    std::vector<int> vec;
    std::deque<int> deq;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.empty()) {
            std::cerr << "Error" << std::endl;
            return 1;
        }
        
        for (size_t j = 0; j < arg.length(); ++j) {
            if (!std::isdigit(arg[j]) && !(j == 0 && arg[j] == '+')) {
                std::cerr << "Error" << std::endl;
                return 1;
            }
        }

        char* endptr = NULL;
        long val = std::strtol(arg.c_str(), &endptr, 10);
        if (endptr != NULL && *endptr != '\0') {
            std::cerr << "Error" << std::endl;
            return 1;
        }

        if (val < 0 || val > 2147483647) {
            std::cerr << "Error" << std::endl;
            return 1;
        }

        int num = static_cast<int>(val);

        // Simple duplicate check (set is banned because ex02 specifies vector and deque)
        for (size_t j = 0; j < vec.size(); ++j) {
            if (vec[j] == num) {
                std::cerr << "Error" << std::endl;
                return 1;
            }
        }

        vec.push_back(num);
        deq.push_back(num);
    }

    std::cout << "Before: ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << (i + 1 == vec.size() ? "" : " ");
    }
    std::cout << std::endl;

    struct timeval start, end;
    
    // Test Vector sort
    gettimeofday(&start, NULL);
    PmergeMe::sortVector(vec);
    gettimeofday(&end, NULL);
    double time_vec = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

    // Test Deque sort
    gettimeofday(&start, NULL);
    PmergeMe::sortDeque(deq);
    gettimeofday(&end, NULL);
    double time_deq = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

    std::cout << "After:  ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << (i + 1 == vec.size() ? "" : " ");
    }
    std::cout << std::endl;

    std::cout << "Time to process a range of " << vec.size() 
              << " elements with std::vector : " << std::fixed << std::setprecision(5) 
              << time_vec << " us" << std::endl;

    std::cout << "Time to process a range of " << deq.size() 
              << " elements with std::deque  : " << std::fixed << std::setprecision(5) 
              << time_deq << " us" << std::endl;

    return 0;
}
