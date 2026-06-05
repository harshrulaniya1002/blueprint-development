# Partition DP - Quick Reference & Cheat Sheet

One-page reference for solving partition DP problems quickly.

---

## Recognition Signals

**You need Partition DP when:**
- Problem asks to "partition", "split", "divide" an array/string
- Subproblems are **independent** after splitting
- You need to try **all possible split points**
- Looking for min/max over all possible partitions
- Keywords: "minimum cuts", "triangulation", "burst", "break"

**Compare with Stock DP:**
- Stock: Track **state changes** (buy → sell → buy → sell)
- Partition: Track **split points** (where to cut?)

---

## DP State Templates

### **1D Partition (Linear)**
```cpp
dp[i] = best result for arr[0...i]

// Transition
for(int j = 0; j < i; j++) {
    if(is_valid_partition(j, i)) {
        dp[i] = optimize(dp[i], combine(dp[j], cost(j, i)));
    }
}
```

**Problems**: Decode Ways, Word Break, Palindrome Partitioning II

---

### **2D Range (Interval) DP**
```cpp
dp[i][j] = best result for range [i...j]

// Build by increasing length
for(int len = 2; len <= n; len++) {
    for(int i = 0; i + len - 1 <= n; i++) {
        int j = i + len - 1;
        dp[i][j] = INF; // or -INF
        
        // Try each split point k
        for(int k = i; k < j; k++) {
            dp[i][j] = optimize(dp[i][j], 
                                combine(dp[i][k], dp[k+1][j], cost));
        }
    }
}
```

**Problems**: Polygon Triangulation, Burst Balloons, Guess Number

---

## Transition Patterns

### **Pattern A: Take Left + Right**
```cpp
dp[i][j] = dp[i][k] + dp[k][j]  // sum subproblems
```
Example: Palindrome Partitioning (cuts)

---

### **Pattern B: Add Middle + Subproblems**
```cpp
dp[i][j] = cost(i, k, j) + dp[i][k] + dp[k][j]
```
Example: Burst Balloons, Polygon Triangulation

---

### **Pattern C: Minimax**
```cpp
dp[i][j] = min over k of: cost(k) + max(dp[i][k-1], dp[k+1][j])
```
Example: Guess Number

---

### **Pattern D: Boolean (Can/Cannot)**
```cpp
dp[i] = OR over all valid splits
if(dp[j] AND is_valid(j, i)):
    dp[i] = true
```
Example: Word Break

---

## Common Loop Structures

### **1D Problem**
```cpp
// Size of partition
for(int i = 1; i <= n; i++) {
    for(int k = 0; k < i; k++) {
        dp[i] = optimize(dp[i], combine(dp[k], cost(k, i)));
    }
}
```
Complexity: **O(n²)**

---

### **2D Problem**
```cpp
// Build by increasing range length
for(int len = 2; len <= n; len++) {
    for(int i = 0; i < n; i++) {
        int j = i + len - 1;
        for(int k = i; k < j; k++) {
            dp[i][j] = optimize(dp[i][j], 
                                combine(dp[i][k], dp[k+1][j], cost));
        }
    }
}
```
Complexity: **O(n³)**

---

## Initialization Guide

| State Type | Base Case | What It Means |
|-----------|-----------|---|
| 1D: Empty | `dp[0] = 0` (or identity) | Empty partition costs 0 |
| 1D: Single | `dp[1] = f(arr[0])` | Single element base cost |
| 2D: Single vertex | `dp[i][i] = 0` | No cost for 1-element range |
| 2D: Invalid | `dp[i][j] = INF` | Before computing |

---

## Debugging Checklist

- [ ] Base cases correct?
- [ ] Loop bounds correct? (especially `j < k` vs `k <= j`)
- [ ] Transition formula makes logical sense?
- [ ] Off-by-one error in indices?
- [ ] Are you building from small to large?
- [ ] Have you tested on small examples (n=2, 3)?
- [ ] Are subproblems actually independent?

---

## Problem-Specific Tips

### **91. Decode Ways**
```
Validity: 1-9 (1 digit), 10-26 (2 digits)
dp[i] = (valid_1 ? dp[i-1] : 0) + (valid_2 ? dp[i-2] : 0)
```

### **139. Word Break**
```
Check if substring is in dictionary with O(1) hash lookup
Convert to Trie if many substrings to check
```

### **132. Palindrome Partitioning II**
```
Step 1: Build is_pal[i][j] table
Step 2: Use partition DP with palindrome check
Order matters: precompute palindromes first!
```

### **1039. Polygon Triangulation**
```
Think: "Make triangle (i, k, j), then solve (i,k) and (k,j)"
Padding not needed here (unlike Burst Balloons)
```

### **375. Guess Number**
```
cost = k + max(dp[i][k-1], dp[k+1][j])
Taking max because opponent chooses worst case for us
```

### **312. Burst Balloons**
```
CRITICAL: Think "burst k LAST" not first!
Add padding (1) at both ends
coins[k_last] = nums[i] * nums[k] * nums[j]
```

---

## Space Optimization

### **1D Problems**
Can't usually optimize below O(n) because you need previous results

### **2D Problems**
Can optimize to O(n²) from O(n²) — already optimal for range DP

### **General Observation**
Most partition DP is inherently O(n²) or O(n³) in both time and space

---

## Comparison: Partition vs Stock vs Other DP

| Type | Dimensions | Loop Count | Example State |
|------|-----------|-----------|---|
| **Stock DP** | 2D | `for i, for buy` | (position, state) |
| **Partition DP** | 2D or 3D | `for len, for i, for k` | (range) or (multi-range) |
| **Sequence DP** | 2D | `for i, for j` | (position, position) |
| **Knapsack** | 2D | `for i, for w` | (item, capacity) |
| **Bitmask DP** | 2D | `for mask, for i` | (state, position) |

---

## Speed Tips

1. **Identify pattern early**: 1D linear or 2D interval?
2. **Initialize correctly**: Prevents wrong answers
3. **Loop order matters**: For 2D, length first!
4. **Test on small examples**: n=2,3,4 reveal issues
5. **Check complexity**: If O(n³) and n=1000, you'll TLE

---

## Keywords to Trigger Partition DP

- Partition, Split, Cut, Break, Divide
- Minimum cuts, Maximum score
- Triangulation, Burst, Pop
- Interleave, Scramble, Rearrange
- Palindromes (when counting partitions)

---

## Related Techniques

- **Palindrome Precomputation**: For Palindrome Partitioning
- **Trie**: For Word Break optimization
- **Interval DP**: Core for range problems
- **Minimax**: For game-theoretic problems
- **Memoization**: Can convert tabulation to memoization

