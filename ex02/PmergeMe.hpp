#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <cstddef>

template <typename Container>
struct ContainerTraits {
    typedef std::vector<std::size_t> IndexContainer;
    typedef std::vector<bool> BoolContainer;
};

template <>
struct ContainerTraits<std::deque<int> > {
    typedef std::deque<std::size_t> IndexContainer;
    typedef std::deque<bool> BoolContainer;
};

class PmergeMe {
public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& rhs);
    ~PmergeMe();

    void run(int argc, char** argv);

private:
    std::vector<int> _original;
    std::vector<int> _vectorSorted;
    std::deque<int>  _dequeSorted;
    std::size_t      _vecComparisons;
    std::size_t      _deqComparisons;
    double           _vecTimeUs;
    double           _deqTimeUs;

    static void parseInput(int argc, char** argv, std::vector<int>& dest);

    template <typename Container>
    static void fordJohnsonSort(Container& container, std::size_t& comparisons);

    template <typename Container>
    static Container recursiveMergeInsert(Container& container, std::size_t& comparisons);

    template <typename Container>
    static void insertPending(Container& mainChain, const Container& pendChain, std::size_t& comparisons);

    template <typename Container>
    static typename ContainerTraits<Container>::IndexContainer generateJacobsthal(std::size_t size);

    template <typename Container>
    static typename ContainerTraits<Container>::IndexContainer buildInsertionOrder(
        const typename ContainerTraits<Container>::IndexContainer& jacobSeq,
        std::size_t pendSize);

    template <typename Container>
    static std::size_t binarySearchPosition(
        const Container& chain,
        int target,
        std::size_t upperBound,
        std::size_t& comparisons);

    static double getMicroseconds();
    void printSequence(const std::vector<int>& seq, const std::string& prefix) const;
    void printBenchmark() const;
};

#endif
