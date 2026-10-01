# 📚 CPP Module 09: Complete Study Guide & Peer Evaluation Defense

This study guide provides a comprehensive breakdown of **C++ Module 09 (STL Advanced: Parsers and Algorithms)**. It covers core theory, line-by-line implementations, peer defense answers, and live code modification prep for 42 evaluations.

---

## 📑 Table of Contents
0. [⚡ 3-Minute Quick Study Guide (TL;DR Cheat Sheet)](#0--3-minute-quick-study-guide-tldr-cheat-sheet)
1. [The Golden Rule: STL Container Selection & Architecture](#1-the-golden-rule-stl-container-selection--architecture)
2. [Exercise 00: BitcoinExchange Deep-Dive](#2-exercise-00-bitcoinexchange-deep-dive)
3. [Exercise 01: Reverse Polish Notation (RPN) Deep-Dive](#3-exercise-01-reverse-polish-notation-rpn-deep-dive)
4. [Exercise 02: PmergeMe (Ford-Johnson Merge-Insertion Sort) Deep-Dive](#4-exercise-02-pmergeme-ford-johnson-merge-insertion-sort-deep-dive)
5. [Top Peer Evaluation Defense Questions & Answers](#5-top-peer-evaluation-defense-questions--answers)
6. [Live Code Modification Practice](#6-live-code-modification-practice)

---

## 0. ⚡ 3-Minute Quick Study Guide (TL;DR Cheat Sheet)

If your peer evaluation starts in 5 minutes, memorize this section:

### 1. The 3 Exercises in One Sentence

| Exercise | What It Does | Container Used | The #1 Trap Evaluators Look For |
| :--- | :--- | :--- | :--- |
| **ex00: `BitcoinExchange`** | Multiplies date rate with value; matches closest previous date | `std::map<std::string, double>` | **Why `upper_bound` instead of `lower_bound`?** How leap years and date bounds are validated. |
| **ex01: `RPN`** | Postfix calculator with $+$, $-$, $*$, $/$ | `std::stack<int>` | **Operand order**: `left - right` (NOT `right - left`). Operands must be strictly $< 10$. Division by zero must error. |
| **ex02: `PmergeMe`** | Ford-Johnson sort minimizing comparisons | `std::vector<int>` & `std::deque<int>` | **Explaining Jacobsthal numbers**: why insertion grouping $(J_k)$ minimizes comparisons in binary search ($2^k - 1$). |

---

### 2. Top 5 Questions Evaluators Ask & Quick Answers

1. **Why do you use `upper_bound(date)` instead of `lower_bound(date)` in `BitcoinExchange`?**
   > *Answer*: `lower_bound(date)` returns the first element $\ge date$. If the exact date is not in the database, `lower_bound` gives a **future** date, not the preceding one. `upper_bound(date)` returns the first date strictly $> date$. Therefore, decrementing the iterator (`--it`) guarantees the greatest date $\le date$ (the closest preceding date).

2. **Why can't you reuse `std::vector` in ex00 or ex01?**
   > *Answer*: The subject strictly forbids using the same container across exercises. We use `std::map` in ex00, `std::stack` in ex01, and `std::vector` + `std::deque` in ex02.

3. **In RPN, what order do you pop operands from the stack?**
   > *Answer*: The first popped element is the **right operand**, and the second popped element is the **left operand**. For non-commutative operations like subtraction and division, `left - right` and `left / right` are mandatory.

4. **What is the Ford-Johnson algorithm and why is it special?**
   > *Answer*: Also called merge-insertion sort, it was designed by Lester Ford Jr. and Selmer Johnson (1959) to minimize the **exact number of comparisons** needed to sort $N$ items. It is close to the theoretical information-theoretic lower bound $\lceil \log_2(N!) \rceil$.

5. **Why do you use Jacobsthal numbers in PmergeMe?**
   > *Answer*: When inserting pending elements into the main chain, binary searching within an array of size $2^k - 1$ takes at most $k$ comparisons. Jacobsthal numbers group pending elements such that the main chain size stays within optimal powers-of-two boundaries, preventing wasted comparisons.

---

## 1. The Golden Rule: STL Container Selection & Architecture

Module 09 evaluates your understanding of container internal mechanics:

```
+-----------------------------------------------------------------------------------+
|                                  STL CONTAINERS                                   |
+------------------------------------+----------------------------------------------+
| Sequence Containers                | Associative & Adapter Containers             |
+------------------------------------+----------------------------------------------+
| std::vector: Contiguous array      | std::map: Red-Black self-balancing BST       |
| std::deque:  Chunked array list    | std::stack: Container adapter (LIFO)         |
+------------------------------------+----------------------------------------------+
```

### Why These Containers Were Chosen:

1. **`ex00`: `std::map<std::string, double>`**
   - **Internal Structure**: Self-balancing Red-Black binary search tree.
   - **Properties**: Keys (`YYYY-MM-DD`) are automatically maintained in sorted lexicographical order.
   - **Complexity**: $O(\log N)$ search, insertion, and deletion.
   - **Key Advantage**: `std::map` provides built-in logarithmic binary search methods (`upper_bound`, `lower_bound`).

2. **`ex01`: `std::stack<int>`**
   - **Internal Structure**: Container adapter wrapping an underlying sequence container (`std::deque` by default).
   - **Properties**: Strict LIFO (Last-In, First-Out) interface (`push()`, `pop()`, `top()`, `empty()`, `size()`).
   - **Complexity**: $O(1)$ constant time for all operations.
   - **Key Advantage**: Matches the formal semantics of a stack-based postfix evaluation machine.

3. **`ex02`: `std::vector<int>` and `std::deque<int>`**
   - **`std::vector`**:
     - Contiguous memory block with $O(1)$ random access (`[]`).
     - Excellent CPU cache locality.
     - Insertion in the middle requires shifting memory ($O(N)$ copies).
   - **`std::deque` (Double-Ended Queue)**:
     - Segmented memory (array of fixed-size chunk buffers).
     - Allows $O(1)$ insertion at both front and back without reallocating existing elements.
     - Slightly higher iterator dereferencing overhead due to two-level pointer lookup.

---

## 2. Exercise 00: BitcoinExchange Deep-Dive

### The Workflow:
1. Parse `data.csv`: Store `date` (string) and `exchange_rate` (double) in `_rates` (`std::map<std::string, double>`).
2. Read the user input file line-by-line.
3. Validate format `date | value`.
4. Validate date (leap year rules, calendar days).
5. Validate value (positive float/int, $0 \le \text{value} \le 1000$).
6. Query exchange rate using `_rates.upper_bound(date)`.
7. Output `date => value = (value * rate)`.

### Line-by-Line Code Breakdown: Closest Date Lookup
```cpp
double BitcoinExchange::_getExchangeRate(const std::string& date) const
{
    // Find the first element with key STRICTLY GREATER than date
    std::map<std::string, double>::const_iterator it = _rates.upper_bound(date);

    // If upper_bound is begin(), then all dates in the database are later than 'date'
    if (it == _rates.begin())
        return -1.0;

    // Step back one position to get the largest date <= 'date'
    --it;
    return it->second;
}
```

#### Why `upper_bound` + `--it` Works Perfectly:
- Case 1: Exact date exists (e.g. `2011-01-03` exists).
  `upper_bound("2011-01-03")` returns iterator pointing to next date (e.g. `2011-01-04`). Decrementing gives `2011-01-03`.
- Case 2: Exact date does not exist (e.g. `2011-01-02` between `2011-01-01` and `2011-01-03`).
  `upper_bound("2011-01-02")` returns `2011-01-03`. Decrementing gives `2011-01-01` (closest previous date).
- Case 3: Date earlier than genesis date (`2009-01-02`).
  `upper_bound("2008-05-01")` returns `_rates.begin()`. Handled as an error (`return -1.0`).

### Leap Year Calculation
```cpp
bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
if (isLeap)
    maxDays = 29;
```

---

## 3. Exercise 01: Reverse Polish Notation (RPN) Deep-Dive

### How Postfix Evaluation Works
In standard infix notation: `(3 + 4) * 2`.
In Reverse Polish Notation (postfix): `3 4 + 2 *`.

Because operators follow operands, **no parentheses or operator precedence rules are needed**. The evaluation algorithm is:
1. Scan tokens from left to right.
2. If token is a number ($0 \dots 9$), `push` it onto the stack.
3. If token is an operator ($+$, $-$, $*$, $/$):
   - Pop `right` operand.
   - Pop `left` operand.
   - Compute `left OP right`.
   - Push result back onto the stack.
4. At end of expression, the stack must contain exactly **1 element** (the answer).

### Crucial Operand Ordering:
```cpp
int right = stack.top();
stack.pop();
int left = stack.top();
stack.pop();

int result = _applyOperation(left, right, token[0]);
stack.push(result);
```
> **Warning**: Subtraction and division are NOT commutative!
> In `5 3 -`, `top()` is `3` (right) and second is `5` (left).
> Result must be `5 - 3 = 2`, NOT `3 - 5 = -2`!

---

## 4. Exercise 02: PmergeMe (Ford-Johnson Merge-Insertion Sort) Deep-Dive

### Theoretical Foundation
Developed in 1959 by Lester Ford Jr. and Selmer M. Johnson, merge-insertion sort was the benchmark minimum-comparison sort for decades.

For $N$ items, the minimum theoretical comparisons is $\lceil \log_2(N!) \rceil$.
- For $N=5$: $\lceil \log_2(120) \rceil = 7$ comparisons.
- Ford-Johnson sorts 5 elements in at most **7 comparisons**.

### The 5 Steps of Ford-Johnson:

```
Step 1: Pair elements and compare
[ 19, 82, 4, 77, 12, 99, 2, 54, 33, 21, 60, 45, 88, 1, 7, 100, 23, 5, 67, 10 ]
   (82, 19), (77, 4), (99, 12), (54, 2), (33, 21), (60, 45), (88, 1), (100, 7), (23, 5), (67, 10)

Step 2: Recursively sort larger elements (main chain)
Larger: [ 82, 77, 99, 54, 33, 60, 88, 100, 23, 67 ]
Sorted: [ 23, 33, 54, 60, 67, 77, 82, 88, 99, 100 ]

Step 3: Insert the first partner element (b1) at index 0 (0 comparisons)
Main Chain: [ b1, a1, a2, a3, ... ]

Step 4: Insert remaining pending elements using Jacobsthal Sequence order
Jacobsthal numbers: 0, 1, 1, 3, 5, 11, 21, 43, 85, 171...
Groups: {b3, b2}, {b5, b4}, {b11, b10, b9, b8, b7, b6}...
Each element is inserted via binary search bounded by its partner's position!

Step 5: Insert odd straggler (if N was odd) via binary search
```

### The Jacobsthal Formula:
$$J_0 = 0,\quad J_1 = 1,\quad J_k = J_{k-1} + 2 J_{k-2}$$

```cpp
std::vector<std::size_t> PmergeMe::_buildJacobSequence(std::size_t count)
{
    std::vector<std::size_t> sequence;
    if (count == 0) return sequence;

    std::vector<std::size_t> jacob;
    jacob.push_back(0);
    jacob.push_back(1);

    while (jacob.back() < count)
    {
        std::size_t nextVal = jacob.back() + 2 * jacob[jacob.size() - 2];
        jacob.push_back(nextVal);
    }

    std::size_t lastIndex = 1;
    for (std::size_t k = 3; k < jacob.size(); ++k)
    {
        std::size_t current = jacob[k];
        std::size_t top = (current <= count) ? current : count;
        for (std::size_t idx = top; idx > lastIndex; --idx)
            sequence.push_back(idx - 1); // Reverse order inside block
        lastIndex = top;
        if (lastIndex >= count)
            break;
    }
    for (std::size_t idx = count; idx > lastIndex; --idx)
        sequence.push_back(idx - 1);

    return sequence;
}
```

### Why Reverse Order within Jacobsthal Blocks?
When inserting $b_3$ before $b_2$:
- $b_3$ is bounded by $a_3$. The sub-array up to $a_3$ currently has size $2^2 - 1 = 3$. Binary search in 3 elements takes at most $\lceil \log_2(3 + 1) \rceil = 2$ comparisons!
- Once $b_3$ is inserted, the main chain expands, but $b_2$ only needs to search up to $a_2$, which is still within the 3-element bound!
- This mathematical guarantee prevents exceeding comparison budgets!

---

## 5. Top Peer Evaluation Defense Questions & Answers

### Q1: Why did you pick `std::vector` and `std::deque` for PmergeMe?
> **Answer**: `std::vector` provides contiguous memory and cache efficiency with fast random access iterators, which speeds up `std::lower_bound` pointer arithmetic. However, inserting in the middle causes $O(N)$ element memory shifts. `std::deque` stores elements across fragmented chunks, allowing efficient insertions without full-array reallocation, but with slightly slower random-access traversal. Comparing them highlights trade-offs between cache locality and insertion reallocation overhead.

### Q2: What happens if `input.txt` in ex00 has an empty line or trailing spaces?
> **Answer**: Empty lines are skipped (`if (line.empty()) continue;`). Leading and trailing whitespaces around date and value strings are stripped using `find_first_not_of` and `find_last_not_of(" \t")`, ensuring robust parsing even with formatted inputs.

### Q3: What happens if an RPN expression has multiple spaces between numbers?
> **Answer**: `std::istringstream stream(expression)` extracts tokens using `operator>>`, which automatically treats any consecutive whitespace characters (spaces, tabs, newlines) as delimiters.

### Q4: Can RPN produce an integer overflow?
> **Answer**: Intermediate results are stored in signed 32-bit `int`. Calculations exceeding `INT_MAX` or below `INT_MIN` cause integer overflow according to C++ standard rules.

### Q5: What is Orthodox Canonical Form and where did you use it?
> **Answer**: Orthodox Canonical Form requires:
> 1. Default constructor
> 2. Copy constructor
> 3. Copy assignment operator (`operator=`)
> 4. Destructor
> Every class (`BitcoinExchange`, `RPN`, `PmergeMe`) implements all four member functions.

---

## 6. Live Code Modification Practice

Be prepared to make these live changes during evaluation:

### Scenario A: Add the modulo operator `%` to RPN
In `RPN.cpp`:
1. Update `_isOperator`:
   ```cpp
   bool RPN::_isOperator(char c) {
       return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
   }
   ```
2. Update `_applyOperation`:
   ```cpp
   case '%':
       if (b == 0) throw std::runtime_error("division by zero");
       return a % b;
   ```

### Scenario B: Reject duplicate numbers in PmergeMe
In `ex02/main.cpp`:
```cpp
#include <set>

// Inside parsing loop:
std::set<int> uniqueCheck;
for (int i = 1; i < argc; ++i) {
    int val = 0;
    if (!parsePositiveInt(argv[i], val) || !uniqueCheck.insert(val).second) {
        std::cerr << "Error" << std::endl;
        return 1;
    }
}
```

### Scenario C: Support values up to 5000 in BitcoinExchange
In `ex00/BitcoinExchange.cpp`:
Change line 191:
```cpp
if (value > 5000.0) // previously 1000.0
```
