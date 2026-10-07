#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <cstddef>

class FordJohnsonMergeInsertSorter {
public:
    FordJohnsonMergeInsertSorter();
    FordJohnsonMergeInsertSorter(const FordJohnsonMergeInsertSorter& other_sorter);
    FordJohnsonMergeInsertSorter& operator=(const FordJohnsonMergeInsertSorter& rhs);
    ~FordJohnsonMergeInsertSorter();

    static void execute_ford_johnson_sort_using_vector(std::vector<int>& sequence_to_sort);
    static void execute_ford_johnson_sort_using_deque(std::deque<int>& sequence_to_sort);

private:
    static std::vector<std::size_t> generate_jacobsthal_insertion_sequence_for_vector(std::size_t pending_elements_count);
    static std::deque<std::size_t> generate_jacobsthal_insertion_sequence_for_deque(std::size_t pending_elements_count);
};

#endif
