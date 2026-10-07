#include "PmergeMe.hpp"
#include <algorithm>
#include <utility>

FordJohnsonMergeInsertSorter::FordJohnsonMergeInsertSorter() {}
FordJohnsonMergeInsertSorter::FordJohnsonMergeInsertSorter(const FordJohnsonMergeInsertSorter& other_sorter) { (void)other_sorter; }
FordJohnsonMergeInsertSorter& FordJohnsonMergeInsertSorter::operator=(const FordJohnsonMergeInsertSorter& rhs) { (void)rhs; return *this; }
FordJohnsonMergeInsertSorter::~FordJohnsonMergeInsertSorter() {}

std::vector<std::size_t> FordJohnsonMergeInsertSorter::generate_jacobsthal_insertion_sequence_for_vector(std::size_t pending_elements_count) {
    std::vector<std::size_t> jacobsthal_sequence, insertion_sequence;
    jacobsthal_sequence.push_back(0); jacobsthal_sequence.push_back(1);
    for (std::size_t i = 2; jacobsthal_sequence.back() < pending_elements_count; ++i) jacobsthal_sequence.push_back(jacobsthal_sequence[i - 1] + 2 * jacobsthal_sequence[i - 2]);
    for (std::size_t i = 2; i < jacobsthal_sequence.size(); ++i) {
        std::size_t current_jacobsthal_number = std::min(jacobsthal_sequence[i], pending_elements_count);
        for (std::size_t j = current_jacobsthal_number; j > jacobsthal_sequence[i - 1]; --j) insertion_sequence.push_back(j);
    }
    return insertion_sequence;
}

void FordJohnsonMergeInsertSorter::execute_ford_johnson_sort_using_vector(std::vector<int>& sequence_to_sort) {
    if (sequence_to_sort.size() < 2) return;
    bool has_unpaired_odd_element = (sequence_to_sort.size() % 2 != 0);
    int unpaired_odd_element = has_unpaired_odd_element ? sequence_to_sort.back() : 0;
    if (has_unpaired_odd_element) sequence_to_sort.pop_back();

    std::vector<std::pair<int, int> > larger_and_smaller_element_pairs;
    for (std::size_t i = 0; i < sequence_to_sort.size(); i += 2) larger_and_smaller_element_pairs.push_back(std::make_pair(std::max(sequence_to_sort[i], sequence_to_sort[i+1]), std::min(sequence_to_sort[i], sequence_to_sort[i+1])));

    std::vector<int> recursive_larger_elements_sequence;
    for (std::size_t i = 0; i < larger_and_smaller_element_pairs.size(); ++i) recursive_larger_elements_sequence.push_back(larger_and_smaller_element_pairs[i].first);
    execute_ford_johnson_sort_using_vector(recursive_larger_elements_sequence);

    std::vector<int> sorted_main_chain, pending_elements_to_insert;
    for (std::size_t i = 0; i < recursive_larger_elements_sequence.size(); ++i) {
        sorted_main_chain.push_back(recursive_larger_elements_sequence[i]);
        for (std::size_t j = 0; j < larger_and_smaller_element_pairs.size(); ++j) {
            if (larger_and_smaller_element_pairs[j].first == recursive_larger_elements_sequence[i]) {
                pending_elements_to_insert.push_back(larger_and_smaller_element_pairs[j].second);
                break;
            }
        }
    }

    if (!pending_elements_to_insert.empty()) {
        sorted_main_chain.insert(sorted_main_chain.begin(), pending_elements_to_insert.front());
        std::vector<std::size_t> insertion_indices = generate_jacobsthal_insertion_sequence_for_vector(pending_elements_to_insert.size());
        for (std::size_t i = 0; i < insertion_indices.size(); ++i) {
            std::size_t index = insertion_indices[i] - 1;
            if (index == 0) continue;
            std::vector<int>::iterator insertion_point = std::lower_bound(sorted_main_chain.begin(), sorted_main_chain.end(), pending_elements_to_insert[index]);
            sorted_main_chain.insert(insertion_point, pending_elements_to_insert[index]);
        }
    }
    if (has_unpaired_odd_element) sorted_main_chain.insert(std::lower_bound(sorted_main_chain.begin(), sorted_main_chain.end(), unpaired_odd_element), unpaired_odd_element);
    sequence_to_sort = sorted_main_chain;
}

std::deque<std::size_t> FordJohnsonMergeInsertSorter::generate_jacobsthal_insertion_sequence_for_deque(std::size_t pending_elements_count) {
    std::deque<std::size_t> jacobsthal_sequence, insertion_sequence;
    jacobsthal_sequence.push_back(0); jacobsthal_sequence.push_back(1);
    for (std::size_t i = 2; jacobsthal_sequence.back() < pending_elements_count; ++i) jacobsthal_sequence.push_back(jacobsthal_sequence[i - 1] + 2 * jacobsthal_sequence[i - 2]);
    for (std::size_t i = 2; i < jacobsthal_sequence.size(); ++i) {
        std::size_t current_jacobsthal_number = std::min(jacobsthal_sequence[i], pending_elements_count);
        for (std::size_t j = current_jacobsthal_number; j > jacobsthal_sequence[i - 1]; --j) insertion_sequence.push_back(j);
    }
    return insertion_sequence;
}

void FordJohnsonMergeInsertSorter::execute_ford_johnson_sort_using_deque(std::deque<int>& sequence_to_sort) {
    if (sequence_to_sort.size() < 2) return;
    bool has_unpaired_odd_element = (sequence_to_sort.size() % 2 != 0);
    int unpaired_odd_element = has_unpaired_odd_element ? sequence_to_sort.back() : 0;
    if (has_unpaired_odd_element) sequence_to_sort.pop_back();

    std::deque<std::pair<int, int> > larger_and_smaller_element_pairs;
    for (std::size_t i = 0; i < sequence_to_sort.size(); i += 2) larger_and_smaller_element_pairs.push_back(std::make_pair(std::max(sequence_to_sort[i], sequence_to_sort[i+1]), std::min(sequence_to_sort[i], sequence_to_sort[i+1])));

    std::deque<int> recursive_larger_elements_sequence;
    for (std::size_t i = 0; i < larger_and_smaller_element_pairs.size(); ++i) recursive_larger_elements_sequence.push_back(larger_and_smaller_element_pairs[i].first);
    execute_ford_johnson_sort_using_deque(recursive_larger_elements_sequence);

    std::deque<int> sorted_main_chain, pending_elements_to_insert;
    for (std::size_t i = 0; i < recursive_larger_elements_sequence.size(); ++i) {
        sorted_main_chain.push_back(recursive_larger_elements_sequence[i]);
        for (std::size_t j = 0; j < larger_and_smaller_element_pairs.size(); ++j) {
            if (larger_and_smaller_element_pairs[j].first == recursive_larger_elements_sequence[i]) {
                pending_elements_to_insert.push_back(larger_and_smaller_element_pairs[j].second);
                break;
            }
        }
    }

    if (!pending_elements_to_insert.empty()) {
        sorted_main_chain.insert(sorted_main_chain.begin(), pending_elements_to_insert.front());
        std::deque<std::size_t> insertion_indices = generate_jacobsthal_insertion_sequence_for_deque(pending_elements_to_insert.size());
        for (std::size_t i = 0; i < insertion_indices.size(); ++i) {
            std::size_t index = insertion_indices[i] - 1;
            if (index == 0) continue;
            std::deque<int>::iterator insertion_point = std::lower_bound(sorted_main_chain.begin(), sorted_main_chain.end(), pending_elements_to_insert[index]);
            sorted_main_chain.insert(insertion_point, pending_elements_to_insert[index]);
        }
    }
    if (has_unpaired_odd_element) sorted_main_chain.insert(std::lower_bound(sorted_main_chain.begin(), sorted_main_chain.end(), unpaired_odd_element), unpaired_odd_element);
    sequence_to_sort = sorted_main_chain;
}
