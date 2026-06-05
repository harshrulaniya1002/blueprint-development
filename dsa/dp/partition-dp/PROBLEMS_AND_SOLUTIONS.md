# Partition DP - Best Problems List & Key Takeaways

Quick reference for the best partition DP problems included in this folder.

---

## Problem List

### **Category 1: Linear 1D Partition**

#### **91. Decode Ways** ⭐ (Medium) - **START HERE**
- **File**: [91_decode_ways.cpp](91_decode_ways.cpp)
- **Core Concept**: At each position, try 1 or 2 digit partitions
- **DP State**: `dp[i]` = ways to decode `s[0...i-1]`
- **Transition**: Take 1 digit + take 2 digits (if valid)
- **Complexity**: O(n) time, O(n) space (or O(1) optimized)
- **Key Learning**: How partition DP works on linear arrays

---

#### **139. Word Break** ⭐ (Medium) - **BUILD INTUITION**
- **File**: [139_word_break.cpp](139_word_break.cpp)
- **Core Concept**: Partition string using dictionary words
- **DP State**: `dp[i]` = can `s[0...i-1]` be partitioned into dict words?
- **Transition**: Try all split points, check if right part is in dict
- **Complexity**: O(n²) time, O(n) space (or better with Trie)
- **Key Learning**: How to combine DP with data structures (set/Trie)

---

#### **132. Palindrome Partitioning II** (Hard) - **ADD PREPROCESSING**
- **File**: [132_palindrome_partition_ii.cpp](132_palindrome_partition_ii.cpp)
- **Core Concept**: Minimum cuts to partition into palindromes
- **DP State**: `dp[i]` = minimum cuts for `s[0...i]`
- **Preprocessing**: Precompute all palindromes using `is_pal[i][j]` table
- **Transition**: Try all split points where right part is palindrome
- **Complexity**: O(n²) time for both precomputation and DP, O(n²) space
- **Key Learning**: When to precompute auxiliary information before DP

---

### **Category 2: Range/Interval DP**

#### **1039. Minimum Score Triangulation of Polygon** ⭐ (Medium) - **LEARN RANGE DP**
- **File**: [1039_polygon_triangulation.cpp](1039_polygon_triangulation.cpp)
- **Core Concept**: Triangulate polygon minimizing score sum
- **DP State**: `dp[i][j]` = minimum score for range [i, j]
- **Transition**: Try each k as one vertex of final triangle, split into [i,k] and [k,j]
- **Build Order**: By increasing range length (len = 2, 3, 4, ...)
- **Complexity**: O(n³) time, O(n²) space
- **Key Learning**: 3-nested-loop pattern for range DP

---

#### **375. Guess Number Higher or Lower II** (Medium) - **MINIMAX STRATEGY**
- **File**: [375_guess_number.cpp](375_guess_number.cpp)
- **Core Concept**: Minimize worst-case cost in guessing game
- **DP State**: `dp[i][j]` = minimum worst-case cost for range [i, j]
- **Transition**: Try each guess k, worst case = k + max(left_subproblem, right_subproblem)
- **Strategy**: Minimax principle — optimize for worst-case scenario
- **Complexity**: O(n³) time, O(n²) space
- **Key Learning**: Game theory meets DP, minimax optimization

---

#### **312. Burst Balloons** ⭐⭐⭐ (Hard) - **MASTER THIS**
- **File**: [312_burst_balloons.cpp](312_burst_balloons.cpp)
- **Core Concept**: Burst balloons maximizing coins
- **DP State**: `dp[i][j]` = max coins from bursting balloons in range (i, j)
- **Critical Insight**: Think "which balloon to burst LAST" not first!
- **Trick**: Add padding (1's) at boundaries to avoid edge cases
- **Transition**: k is burst last, coins = `nums[i]*nums[k]*nums[j] + dp[i][k] + dp[k][j]`
- **Complexity**: O(n³) time, O(n²) space
- **Key Learning**: Sometimes reversing the thinking (last instead of first) solves dependency issues

---

## Difficulty Progression

1. **Easy/Warm-up**: 91. Decode Ways
2. **Medium/Reinforce**: 139. Word Break
3. **Medium/Challenging**: 375. Guess Number
4. **Medium/Range DP**: 1039. Polygon Triangulation
5. **Hard/Add Preprocessing**: 132. Palindrome Partitioning II
6. **Hard/Master Pattern**: 312. Burst Balloons

---

## Problem Comparison Matrix

| Problem | Pattern | Loop Nesting | Base Case | Key Trick |
|---------|---------|--------------|-----------|-----------|
| 91 | 1D Linear | 1 | Single char | Take 1 or 2 digits |
| 139 | 1D Linear | 2 | Empty string | Dictionary lookup |
| 132 | 1D + Preproc | 2 main + 3 prep | Single char | Precompute palindromes |
| 1039 | Range | 3 | Single vertex | Try each k as final |
| 375 | Range/Minimax | 3 | Single number | max(left, right) |
| 312 | Range/Last | 3 + padding | Boundaries | Burst LAST not first |

---

## Key Takeaways by Problem

### Decode Ways
```
How to think:
  At each position, ask: "Should I take 1 digit or 2?"
  Both are valid partitions if they form valid codes

Pitfall:
  Don't forget to check validity (1-26 for 2-digit codes)
```

### Word Break
```
How to think:
  At each position, ask: "Can I find a dictionary word here?"
  Try all split points and pick any that works

Optimization:
  Use Trie instead of HashSet for potentially better average case
```

### Palindrome Partitioning II
```
How to think:
  First determine which substrings are palindromes
  Then use partition DP to find minimum cuts

Complexity consideration:
  Two stages: O(n²) preprocessing + O(n²) DP
  Total remains O(n²) due to adding, not multiplying
```

### Polygon Triangulation
```
How to think:
  Imagine "committing" to one triangle at each step
  This triangle divides problem into two independent subproblems

Loop order:
  MUST build by increasing range length (not starting position)
  Otherwise you reference subproblems not yet computed
```

### Guess Number
```
How to think:
  I want to guarantee a win in worst-case
  Pick a guess that minimizes my worst-case exposure

Minimax insight:
  max(left, right) = worst case after my guess
  min across all guesses = best strategy
```

### Burst Balloons
```
How to think:
  Instead of "which balloon to burst first?", ask 
  "which balloon to burst LAST?"

Why this matters:
  When k is burst last, its neighbors are GUARANTEED to be i and j
  When k is burst first, its neighbors change (dependency problem!)

Padding trick:
  Add 1's at boundaries so all calculations are consistent
  The 1's never get burst, act as permanent walls
```

---

## Common Patterns Across All Problems

✅ **Pattern Recognition**:
- All use bottom-up DP with memoization
- All build from smaller subproblems to larger ones
- All try multiple choices and pick the best

✅ **Loop Nesting**:
- 1D problems: 1-2 nested loops
- Range problems: 3 nested loops (i, j, k) — this is standard

✅ **Memoization Check**:
- Always before computing subproblems
- Prevents redundant calculations

✅ **Base Cases**:
- Single elements/ranges: usually 0 or identity value
- Empty cases: usually 0

---

## Related Concepts

If you want to go deeper:
- **Memoization vs Tabulation**: These solutions use tabulation (bottom-up)
- **State Space**: Understanding what each dimension represents
- **Interval DP**: The core technique for range-based problems
- **Minimax**: Used in 375, fundamental in game theory algorithms

---

## Quick Mental Checklist Before Coding

- [ ] Identified the partition/split structure?
- [ ] Defined DP state clearly (what does dp[i][j] mean)?
- [ ] Considered all possible split points?
- [ ] Handled base cases (single elements)?
- [ ] Loop order correct (especially for range DP)?
- [ ] Verified transition formula makes sense?
- [ ] Checked complexity is acceptable?
- [ ] Tested on small examples mentally?

---

## Next Steps

1. ✅ Read README.md to understand concepts
2. ✅ Study problems in order: 91 → 139 → 1039 → 375 → 312
3. ✅ For each problem, understand the DP state and transitions
4. ✅ Try coding each without looking at solution first
5. ✅ Identify the partition structure in each problem
6. ✅ Practice on other similar problems from LeetCode
7. ✅ Eventually solve 132 (hardest non-trivial one)

