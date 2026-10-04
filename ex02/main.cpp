#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    try {
        PmergeMe sorter;
        sorter.run(argc, argv);
    } catch (const std::exception&) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    return 0;
}
