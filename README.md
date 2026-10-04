*This project has been created as part of the 42 curriculum by alnassar.*

# C++ Module 09: STL Advanced (Parsers and Algorithms)

## Description
**C++ Module 09** is the capstone project of the 42 C++ Piscine. The goal of this module is to master the **Standard Template Library (STL)** under the strict **C++98 standard**, focusing on associative containers, container adapters, sequence containers, constrained parsing algorithms, and theoretical comparison-optimal sorting.

The project is structured into three distinct exercises:
1. **`ex00: Bitcoin Exchange` (`btc`)**: An exchange valuation tool that parses historical rate databases, validates user queries against complex temporal constraints, and finds the exact or closest preceding exchange rate.
2. **`ex01: Reverse Polish Notation` (`RPN`)**: A postfix expression evaluator that processes single-digit operands and mathematical operators using a stack-based execution model.
3. **`ex02: PmergeMe` (`PmergeMe`)**: An implementation of the **Ford-Johnson algorithm** (merge-insertion sort) designed to sort positive integer sequences using two distinct sequence containers (`std::vector` and `std::deque`) while minimizing total comparisons via Jacobsthal number sequences.

### ⚠️ The Golden Constraint of Module 09
- At least one container must be used per exercise.
- **No container chosen in an exercise may be reused in another exercise.**
- Each container choice must be justified during peer evaluation.

| Exercise | Deliverable | Container(s) Chosen | Justification Summary |
| :--- | :--- | :--- | :--- |
| **`ex00`** | `btc` | `std::map<std::string, double>` | Associative key-value Red-Black BST keeping dates lexicographically ordered; allows $O(\log N)$ searches using `upper_bound()`. |
| **`ex01`** | `RPN` | `std::stack<int>` | LIFO adapter providing strict $O(1)$ push/pop semantics matching the postfix stack evaluation machine. |
| **`ex02`** | `PmergeMe` | `std::vector<int>` & `std::deque<int>` | Sequence containers providing $O(1)$ random-access iterators required for binary search insertion. |

---

## Instructions

### Prerequisites
- Compiler: `c++` or `g++` supporting C++98.
- Compilation Flags: `-Wall -Wextra -Werror -std=c++98`.

### Compilation & Build Rules
Each exercise includes its own `Makefile` adhering to standard 42 rules:
- `make`: Compiles the binary.
- `make clean`: Removes intermediate object files (`.o`).
- `make fclean`: Removes object files and the compiled executable.
- `make re`: Performs a clean rebuild (`fclean` followed by `make`).

```bash
# Exercise 00: Bitcoin Exchange
cd ex00
make
./btc input.txt

# Exercise 01: Reverse Polish Notation
cd ../ex01
make
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"

# Exercise 02: PmergeMe
cd ../ex02
make
./PmergeMe 3 5 9 7 4
```

---

## Algorithm Explanation: Constrained Decoding & Ford-Johnson Sorting

### 1. Constrained Parsing & Decoding (Exercises 00 & 01)
- **`ex00` Temporal Database Decoding**:
  - Validates `YYYY-MM-DD` calendar constraints, including month boundary checks and leap-year calculations:
    $$\text{isLeapYear} = (\text{year} \pmod 4 = 0 \land \text{year} \pmod{100} \ne 0) \lor (\text{year} \pmod{400} = 0)$$
  - Employs constrained numerical decoding ensuring input values are strictly within $[0, 1000]$ and formatting rejects multiple decimals or ambiguous signs.
  - Queries `std::map::upper_bound(date)`. If the date is earlier than all database records, an error is reported; otherwise, decrementing the iterator yields the closest preceding historical rate.
- **`ex01` Stack-Based Postfix Decoding**:
  - Each input token is decoded sequentially. Operands are strictly constrained to single decimal digits ($0 \le d \le 9$).
  - When an operator is encountered, operands are popped in LIFO order ($b$ then $a$) and evaluated as $a \text{ op } b$ (preserving non-commutative operations like subtraction and division).
  - Division by zero and malformed expressions trigger immediate `Error` termination.

### 2. Ford-Johnson Merge-Insertion Sort (Exercise 02)
The Ford-Johnson algorithm sorts $N$ elements by minimizing the total number of comparisons, approaching the theoretical information-theoretic bound:
$$\lceil \log_2(N!) \rceil$$

The algorithm proceeds in five distinct phases:
1. **Pairwise Comparison**: The $N$ elements are grouped into $\lfloor N / 2 \rfloor$ disjoint pairs. Each pair is compared once, placing the larger element into `mainChain` and the smaller into `pendChain`. If $N$ is odd, the unpaired straggler is placed at the end of `pendChain`.
2. **Recursive Main Chain Sort**: The `mainChain` of larger elements is recursively sorted using the same Ford-Johnson algorithm until the base cases ($N \le 2$) are reached.
3. **Pend Chain Re-alignment**: The `pendChain` elements are permuted to preserve their original pairing with the now-sorted `mainChain`.
4. **Trivial Insertion ($b_1$)**: The first pending element $b_1$ (partner of the smallest main-chain element $a_1$) is inserted at index 0 of `mainChain` without any comparison, since $b_1 \le a_1$.
5. **Jacobsthal Group Insertion**: The remaining pending elements ($b_2, b_3, \dots$) are inserted in optimal batches governed by the **Jacobsthal recurrence**:
   $$J_0 = 0,\quad J_1 = 1,\quad J_n = J_{n-1} + 2J_{n-2}$$
   $$(J_n) = 0, 1, 1, 3, 5, 11, 21, 43, 85, 171, \dots$$
   Within each Jacobsthal interval, elements are inserted in reverse order ($J_k$ down to $J_{k-1} + 1$). Because each $b_i$ is known to be $\le a_i$, binary search insertion is bounded strictly up to the current position of $a_i$ in `mainChain`, guaranteeing at most $k$ comparisons per insertion.

---

## Design Decisions

1. **Strict Container Isolation Across Exercises**:
   - `ex00`: `std::map<std::string, double>`
   - `ex01`: `std::stack<int>`
   - `ex02`: `std::vector<int>` and `std::deque<int>`
   - This ensures complete compliance with the evaluation sheet rule prohibiting container reuse.
2. **Template Genericity & Traits Architecture (`ex02`)**:
   - Rather than duplicating 200+ lines of sorting logic for `vector` and `deque`, `PmergeMe` implements the algorithm once using a templated method.
   - To prevent container pollution (e.g., using `std::vector<size_t>` inside deque sorting), a `ContainerTraits` template specialization supplies the exact matching index container (`std::vector<size_t>` for vector, `std::deque<size_t>` for deque).
3. **Orthodox Canonical Class Form**:
   - All classes (`BitcoinExchange`, `RPN`, `PmergeMe`) strictly implement Default Constructors, Copy Constructors, Copy Assignment Operators, and Destructors.
4. **POSIX Microsecond Profiling**:
   - High-precision wall-clock timing is achieved via `gettimeofday()`, converting elapsed time to microseconds with factor $1000000.0$.

---

## Performance Analysis

### Benchmark: 5 Elements
- **Input**: `3 5 9 7 4`
- **Output**: `3 4 5 7 9`
- **Vector Comparisons**: `7`
- **Deque Comparisons**: `7`
- **Theoretical Minimum**: $\lceil \log_2(5!) \rceil = \lceil \log_2(120) \rceil = 7$ comparisons.
- **Result**: The algorithm achieves the exact theoretical minimum bound.

### Benchmark: 3000 Elements
- **Test Command**: `./PmergeMe $(shuf -i 1-100000 -n 3000 | tr "\n" " ")`
- **Comparisons**: `30446` (optimal bound $\log_2(3000!) \approx 30330$).
- **Vector Execution Time**: $\sim 8257\ \mu\text{s}$ ($\sim 8.2\text{ ms}$).
- **Deque Execution Time**: $\sim 93849\ \mu\text{s}$ ($\sim 93.8\text{ ms}$).

### Speed & Memory Differences Between `std::vector` and `std::deque`
- **`std::vector` (Contiguous Memory)**:
  - Elements reside in a single contiguous memory block.
  - Excellent CPU cache locality: during sequential access and binary searches, hardware prefetchers load adjacent elements into L1/L2 cache lines, resulting in minimal cache misses.
- **`std::deque` (Segmented Block Memory)**:
  - Elements reside in multiple fixed-size chunks indexed by a central map of pointers.
  - Every random access (`deque[mid]`) requires double pointer dereferencing (map lookup + chunk index). During binary search jumps, this indirection introduces frequent cache misses, explaining why `std::deque` takes roughly $10\times$ longer than `std::vector` for 3000 elements.

---

## Challenges Faced & Solutions

1. **Preventing Cross-Container Contamination in `ex02`**:
   - *Challenge*: Using `std::vector` to hold Jacobsthal indices during deque sorting violates strict container separation.
   - *Solution*: Designed a `ContainerTraits` template metaprogramming struct to dynamically map `std::deque<int>` to `std::deque<size_t>` and `std::deque<bool>`.
2. **Integer Overflow Detection**:
   - *Challenge*: `std::strtol()` clamps values exceeding `LONG_MAX` on 32-bit/64-bit systems without throwing exceptions.
   - *Solution*: Reset `errno = 0` prior to parsing and explicitly verify `errno == ERANGE` and `val <= INT_MAX`.
3. **Jacobsthal Search Boundary Capping**:
   - *Challenge*: Uncapped search limits can trigger out-of-bounds binary search indices or redundant comparisons.
   - *Solution*: Bounded the binary search range using $\min(\text{searchLimit} - 1, \text{mainChain.size()})$, precisely matching Ford-Johnson's comparison limits.

---

## Testing Strategy

The implementation was verified using multi-level testing:
1. **Compilation Verification**:
   - Compiled with `c++ -Wall -Wextra -Werror -std=c++98`. Zero warnings, zero errors.
2. **Evaluation Sheet Formula Tests (`ex01`)**:
   - `8 9 * 9 - 9 - 9 - 4 - 1 +` $\rightarrow$ `42`
   - `9 8 * 4 * 4 / 2 + 9 - 8 - 8 - 1 - 6 -` $\rightarrow$ `42`
   - `1 2 * 2 / 2 + 5 * 6 - 1 3 * - 4 5 * * 8 /` $\rightarrow$ `15`
3. **Database & Edge Case Handling (`ex00`)**:
   - Verified leap year handling (`2012-02-29` accepted, `2011-02-29` rejected).
   - Validated bounds ($<0$ rejected, $>1000$ rejected, malformed dates rejected).
   - Confirmed nearest lower date matching on `data.csv`.
4. **Stress & Randomization Testing (`ex02`)**:
   - Validated monotonic ascending order across 3000 randomized integers via Python automated scripts.
   - Tested corner cases: single element, two elements, sorted input, reverse sorted input, duplicate rejection, and negative value rejection.

---

## Example Usage

### Exercise 00: Bitcoin Exchange
```bash
$ cd ex00
$ ./btc input.txt
2011-01-01 => 3 = 0.9
2011-01-01 => 2 = 0.6
2011-01-01 => 1 = 0.3
2011-01-01 => 1.2 = 0.36
2011-01-02 => 1 = 0.3
Error: not a positive number.
Error: bad input => 2001-42-42
2012-01-11 => 1 = 7.1
Error: too large a number.
```

### Exercise 01: Reverse Polish Notation
```bash
$ cd ex01
$ ./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
42
$ ./RPN "1 2 * 2 / 2 + 5 * 6 - 1 3 * - 4 5 * * 8 /"
15
$ ./RPN "(1 + 1)"
Error
$ ./RPN "4 0 /"
Error
```

### Exercise 02: PmergeMe
```bash
$ cd ex02
$ ./PmergeMe 3 5 9 7 4
Before:		3 5 9 7 4
After:		3 4 5 7 9
Time to process a range of 5 elements with std::vector : 19.00000 us
Time to process a range of 5 elements with std::deque  : 16.00000 us
Number of comparisons with std::vector : 7
Number of comparisons with std::deque  : 7
```

---

## Resources

### References
- **Ford, Lester R., and Selmer M. Johnson (1959)**: *A Tournament Problem*, The American Mathematical Monthly, 66(5), 387–389.
- **Knuth, Donald E.**: *The Art of Computer Programming, Volume 3: Sorting and Searching* (Section 5.3.1: Minimum-Comparison Sorting).
- **ISO/IEC 14882:1998**: *Standard for the C++ Programming Language (C++98)*.
- **C++ Reference Documentation**: [cppreference.com - STL Containers](https://en.cppreference.com/w/cpp/container).

### AI Usage Disclosure
In accordance with 42 academic integrity guidelines, AI was utilized during this project for:
1. **Architectural Analysis**: Reviewing the 42 Beirut peer reference project to identify its templated trait design and theoretical comparison bounds.
2. **Comparison Optimization**: Identifying the off-by-one search boundary issue in `insertPending()` to reach the exact theoretical minimum of 7 comparisons for 5 elements.
3. **Documentation & Formatting**: Structuring this comprehensive `README.md` and evaluation study guides in accordance with 42 curriculum standards.
All C++ source files were verified and compiled under strict C++98 standards (`-Wall -Wextra -Werror -std=c++98`).
