#include "PmergeMe.hpp"
#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <iomanip>

int main(int command_line_argument_count, char** command_line_arguments) {
    if (command_line_argument_count < 2) { std::cerr << "Error" << std::endl; return 1; }
    std::vector<int> initial_vector_sequence; std::deque<int> initial_deque_sequence;
    for (int i = 1; i < command_line_argument_count; ++i) {
        std::string current_string_argument = command_line_arguments[i];
        if (current_string_argument.empty()) { std::cerr << "Error" << std::endl; return 1; }
        for (std::size_t j = 0; j < current_string_argument.length(); ++j) {
            if (!std::isdigit(static_cast<unsigned char>(current_string_argument[j])) && !(j == 0 && current_string_argument[j] == '+')) { std::cerr << "Error" << std::endl; return 1; }
        }
        char* end_pointer = NULL;
        long parsed_long_value = std::strtol(current_string_argument.c_str(), &end_pointer, 10);
        if (end_pointer != NULL && *end_pointer != '\0') { std::cerr << "Error" << std::endl; return 1; }
        if (parsed_long_value < 0 || parsed_long_value > 2147483647) { std::cerr << "Error" << std::endl; return 1; }
        int parsed_integer_value = static_cast<int>(parsed_long_value);
        for (std::size_t j = 0; j < initial_vector_sequence.size(); ++j) {
            if (initial_vector_sequence[j] == parsed_integer_value) { std::cerr << "Error" << std::endl; return 1; }
        }
        initial_vector_sequence.push_back(parsed_integer_value); initial_deque_sequence.push_back(parsed_integer_value);
    }
    std::cout << "Before: ";
    for (std::size_t i = 0; i < initial_vector_sequence.size(); ++i) std::cout << initial_vector_sequence[i] << (i + 1 == initial_vector_sequence.size() ? "" : " ");
    std::cout << std::endl;

    struct timeval start_time_structure, end_time_structure;
    gettimeofday(&start_time_structure, NULL);
    FordJohnsonMergeInsertSorter::execute_ford_johnson_sort_using_vector(initial_vector_sequence);
    gettimeofday(&end_time_structure, NULL);
    double vector_sorting_time_in_microseconds = (end_time_structure.tv_sec - start_time_structure.tv_sec) * 1000000.0 + (end_time_structure.tv_usec - start_time_structure.tv_usec);

    gettimeofday(&start_time_structure, NULL);
    FordJohnsonMergeInsertSorter::execute_ford_johnson_sort_using_deque(initial_deque_sequence);
    gettimeofday(&end_time_structure, NULL);
    double deque_sorting_time_in_microseconds = (end_time_structure.tv_sec - start_time_structure.tv_sec) * 1000000.0 + (end_time_structure.tv_usec - start_time_structure.tv_usec);

    std::cout << "After:  ";
    for (std::size_t i = 0; i < initial_vector_sequence.size(); ++i) std::cout << initial_vector_sequence[i] << (i + 1 == initial_vector_sequence.size() ? "" : " ");
    std::cout << std::endl;
    std::cout << "Time to process a range of " << initial_vector_sequence.size() << " elements with std::vector : " << std::fixed << std::setprecision(5) << vector_sorting_time_in_microseconds << " us" << std::endl;
    std::cout << "Time to process a range of " << initial_deque_sequence.size() << " elements with std::deque  : " << std::fixed << std::setprecision(5) << deque_sorting_time_in_microseconds << " us" << std::endl;
    return 0;
}
