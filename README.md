*this project was done by alnassar..*

# C++ - Module 09: STL Advanced (Parsers and Algorithms)

## Description
**C++ Module 09** is the capstone project of the 42 C++ Piscine. It explores advanced applications of the **Standard Template Library (STL)** in C++98, focusing on real-world parsing, abstract data structures, and specialized algorithms.

### ⚠️ The Golden Constraint of Module 09
Unlike earlier modules:
1. **You must use at least one container per exercise.**
2. **Once a container is used in one exercise, it cannot be used in another!**
3. Each container choice must be fully justified during evaluation.

### Containers Used in this Repository:
| Exercise | Exercise Name | Container(s) Used | Why This Container? |
| :--- | :--- | :--- | :--- |
| **`ex00`** | **Bitcoin Exchange** | `std::map<std::string, double>` | Key-value store indexed by date (`YYYY-MM-DD`). Automatically keeps dates lexicographically sorted, providing $O(\log N)$ logarithmic lookup and `upper_bound()` for preceding date matching. |
| **`ex01`** | **Reverse Polish Notation** | `std::stack<int>` | LIFO (Last-In, First-Out) adapter tailored for postfix expression evaluation. Evaluates operations strictly in $O(1)$ push/pop cycles. |
| **`ex02`** | **PmergeMe** | `std::vector<int>` & `std::deque<int>` | Subject explicitly requires sorting with **two different sequence containers**. `std::vector` offers contiguous memory with fast random-access; `std::deque` provides chunked segmented buffers with fast front/back insertions. |

---

## 📑 Summary of Exercises

| Exercise | Primary Topic | Deliverables | Key Requirements & Evaluation Focus |
| :--- | :--- | :--- | :--- |
| **[ex00: Bitcoin Exchange](ex00/)** | Associative Containers & CSV Parsing | `btc`, `Makefile`, `main.cpp`, `BitcoinExchange.hpp`, `BitcoinExchange.cpp`, `data.csv` | Parse database CSV; evaluate queries (`date \| value`); match lower/closest previous date; handle leap years, negative values, and overflows. |
| **[ex01: Reverse Polish Notation](ex01/)** | Container Adapters & Postfix Parsing | `RPN`, `Makefile`, `main.cpp`, `RPN.hpp`, `RPN.cpp` | Evaluate RPN expression from CLI string; operands must be $< 10$; handle division by zero and invalid syntax; output result or `Error`. |
| **[ex02: PmergeMe](ex02/)** | Merge-Insertion Sort & Performance Benchmarks | `PmergeMe`, `Makefile`, `main.cpp`, `PmergeMe.hpp`, `PmergeMe.cpp` | Implement the **Ford-Johnson algorithm** using Jacobsthal insertion order; sort positive integers using `std::vector` and `std::deque`; display timings in $\mu\text{s}$. |

---

## 🛠️ Exercises Overview

### [Exercise 00: Bitcoin Exchange](ex00/)
- **Binary**: `btc`
- **Container**: `std::map<std::string, double>`
- **Behavior**:
  - Loads historical exchange rates from `data.csv` formatted as `date,exchange_rate`.
  - Takes an input file argument containing lines formatted as `date | value`.
  - If the requested date exists, uses its exact exchange rate.
  - If the date does not exist, finds the closest **preceding date** using `std::map::upper_bound()` and stepping back one iterator position (`--it`).
  - Strict validation:
    - Date format: `YYYY-MM-DD`, valid month (1–12), valid days per month, leap year detection.
    - Value bounds: must be a positive float or integer between `0` and `1000`.
- **Usage**:
  ```bash
  cd ex00
  make
  ./btc input.txt
  ```

---

### [Exercise 01: Reverse Polish Notation](ex01/)
- **Binary**: `RPN`
- **Container**: `std::stack<int>`
- **Behavior**:
  - Evaluates an arithmetic expression written in Reverse Polish Notation (postfix).
  - Supported operations: `+`, `-`, `*`, `/`.
  - Input tokens are separated by spaces.
  - Each input operand must strictly be a single digit ($0 \le x < 10$).
  - Results and intermediate calculations may exceed 9 and are signed integers.
  - Errors (insufficient operands, division by zero, trailing operands, non-digit/operator tokens) print `Error`.
- **Usage**:
  ```bash
  cd ex01
  make
  ./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"   # Output: 42
  ./RPN "7 7 * 7 -"                   # Output: 42
  ./RPN "1 2 * 2 / 2 * 2 4 - +"       # Output: 0
  ./RPN "(1 + 1)"                     # Output: Error
  ```

---

### [Exercise 02: PmergeMe](ex02/)
- **Binary**: `PmergeMe`
- **Containers**: `std::vector<int>` and `std::deque<int>`
- **Behavior**:
  - Implements the **Ford-Johnson algorithm** (also known as the **merge-insertion sort**).
  - Designed specifically to minimize the number of element comparisons:
    1. Group the $N$ elements into $\lfloor N/2 \rfloor$ pairs and compare each pair.
    2. Recursively sort the larger elements of each pair to form the **main chain**.
    3. Insert the smallest element's partner ($b_1$) at the front of the main chain without comparison.
    4. Group the remaining pending elements ($b_i$) according to the **Jacobsthal number sequence** ($J_k = J_{k-1} + 2J_{k-2}$).
    5. Insert pending elements in reverse order within each Jacobsthal block via binary insertion (`std::lower_bound`) bounded by their corresponding partner's position.
    6. Insert the odd straggler (if $N$ is odd) via binary search.
  - Benchmarks the execution time for both containers with microsecond resolution.
- **Usage**:
  ```bash
  cd ex02
  make
  ./PmergeMe 3 5 9 7 4
  ./PmergeMe $(shuf -i 1-100000 -n 3000 | tr "\n" " ")
  ```

---

## 📋 Evaluation Sheet Checklist

- [x] **Prerequisites**:
  - Compiles cleanly with `c++ -Wall -Wextra -Werror -std=c++98`.
  - Zero memory leaks.
  - No containers reused across exercises (ex00: `map`, ex01: `stack`, ex02: `vector` + `deque`).
  - Standard Orthodox Canonical Form applied to all classes.

- [x] **Exercise 00**:
  - Header files, implementation files, and Makefile present.
  - Uses `std::map`.
  - Correctly matches closest previous date using `upper_bound()`.
  - Handles invalid dates, leap years, non-positive values, and values $> 1000$.

- [x] **Exercise 01**:
  - Header files, implementation files, and Makefile present.
  - Uses `std::stack`.
  - Operands strictly $< 10$.
  - Handles division by zero, invalid characters, and stack underflow/leftovers.

- [x] **Exercise 02**:
  - Header files, implementation files, and Makefile present.
  - Uses two different containers (`std::vector` and `std::deque`).
  - Ford-Johnson merge-insertion sort correctly implemented (Jacobsthal sequence).
  - Validates positive integers, rejects non-numeric/negative/overflow inputs.
  - Displays `Before:`, `After:`, and precise execution timings for both containers.
