# Partition DP - Concepts, Intuitions & Problems

Partition DP is a dynamic programming technique where you **split a problem into independent subproblems** and combine their results. It's fundamentally different from the stock problems' "state machine" approach.

---

## Core Concept

**Partition DP** = Split an array/string/range into parts → Solve each part independently → Combine results

Think of it as: **"What's the best way to partition/split this input?"**

Compare with Stock DP:
- **Stock DP**: Track state changes (holding/not holding, buy/sell)
- **Partition DP**: Consider different split points and pick the best one

---

## Key Intuitions

### 1. **"Cut/Split" Thinking**
At each position, you ask: **"Should I cut here?"**
- If yes, solve left part + right part
- If no, continue and try next position

Example: "abc" can be partitioned as:
- "a" | "bc"
- "ab" | "c"
- "a" | "b" | "c"

---

### 2. **Range DP**
Problems often use `dp[i][j]` to represent "the best solution for the subproblem from index i to j"

```cpp
dp[i][j] = best result for arr[i...j]

To compute dp[i][j]:
    Try all split points k where i <= k < j:
        dp[i][j] = min/max(dp[i][j], combine(dp[i][k], dp[k+1][j]))
```

---

### 3. **Interval DP Pattern**
```
For each subproblem of length L:
    For each starting position i:
        For each split point k:
            dp[i][i+L-1] = optimize(dp[i][k], dp[k+1][i+L-1])
```

This is the classic 3-nested-loop pattern for range DP.

---

## When to Use Partition DP

✅ **Signs you need Partition DP:**
1. Problem asks for "minimum cost" or "maximum value" by **partitioning/splitting**
2. You need to try **all possible ways to cut/divide** the input
3. Subproblems are **independent** after splitting
4. Problems mention: "palindrome partitioning", "word break", "burst balloons", "triangulation"

❌ **Don't use if:**
- You're tracking state changes (use State Machine DP like stocks)
- Problem is about sequences without splits (use sequence DP)

---

## General Solution Template

### **Pattern 1: 1D Partition (Linear Array)**
```cpp
// dp[i] = best result for arr[0...i]
int dp[n];
dp[0] = base_case;

for(int i = 1; i < n; i++) {
    dp[i] = INF; // or -INF
    for(int k = 0; k < i; k++) {
        // Try splitting at position k
        dp[i] = optimize(dp[i], combine(dp[k], cost(k, i)));
    }
}
return dp[n-1];
```

---

### **Pattern 2: 2D Range DP (Interval)**
```cpp
// dp[i][j] = best result for range [i...j]
int dp[n][n];

// Initialize base cases (single elements)
for(int i = 0; i < n; i++) {
    dp[i][i] = base_case;
}

// Build up by increasing range length
for(int len = 2; len <= n; len++) {
    for(int i = 0; i + len - 1 < n; i++) {
        int j = i + len - 1;
        dp[i][j] = INF; // or -INF
        
        for(int k = i; k < j; k++) {
            // Split at k: combine [i...k] and [k+1...j]
            dp[i][j] = optimize(dp[i][j], 
                                combine(dp[i][k], dp[k+1][j]));
        }
    }
}
return dp[0][n-1];
```

---

## Problem Categories

### **Category 1: String Partitioning**

#### **91. Decode Ways** ⭐ (Medium)
**Concept**: How many ways to decode a string?
- "12" → "1,2" or "12" (2 ways)
- At each position, try taking 1 or 2 characters

**DP State**: `dp[i]` = ways to decode `str[0...i-1]`

**Transition**:
```
if(str[i-1] is valid single digit): dp[i] += dp[i-1]
if(str[i-2:i] is valid two digits): dp[i] += dp[i-2]
```

**Complexity**: O(n) time, O(n) space

---

#### **139. Word Break** ⭐ (Medium)
**Concept**: Can you split a string into words from a dictionary?
- "catsandcatsdog" with dict={"cat","cats","and","dog"} → True

**DP State**: `dp[i]` = can `str[0...i-1]` be split into dictionary words?

**Transition**:
```
For each position j < i:
    if(dp[j] and str[j...i-1] in dict):
        dp[i] = true
```

**Complexity**: O(n²) time, O(n) space (or O(n*m) with trie)

---

#### **132. Palindrome Partitioning II** (Hard)
**Concept**: Minimum cuts to partition string into palindromes?
- "nitin" → "nitin" (0 cuts) or "n|i|t|i|n" (4 cuts) → answer is 0

**DP State**: `dp[i]` = minimum cuts for `str[0...i]`

**Transition**:
```
if(str[0...i] is palindrome): dp[i] = 0
else:
    for each k < i:
        if(str[k+1...i] is palindrome):
            dp[i] = min(dp[i], dp[k] + 1)
```

**Complexity**: O(n²) time, O(n²) space (with palindrome precomputation)

---

### **Category 2: Interval DP (Range Optimization)**

#### **312. Burst Balloons** ⭐⭐ (Hard)
**Concept**: Burst balloons to maximize coins?
- Burst order matters: when you burst balloon i, neighbors become adjacent
- nums = [3,1,5,8] → burst 1 last → earn 3*1*5 = 15 coins

**DP State**: `dp[i][j]` = max coins bursting balloons in range (i, j)

**Transition**:
```
Try bursting each balloon k last in range (i, j):
    dp[i][j] = max(dp[i][j], 
                   dp[i][k] + nums[i]*nums[k]*nums[j] + dp[k][j])
```

**Key Insight**: Think "which balloon to burst LAST" not "first" — solves the dependency problem!

**Complexity**: O(n³) time, O(n²) space

---

#### **1039. Minimum Score Triangulation of Polygon** (Medium)
**Concept**: Minimum score to triangulate polygon?
- Breaking a polygon into triangles, each triangle has score = product of three sides

**DP State**: `dp[i][j]` = minimum score to triangulate polygon from vertex i to j

**Transition**:
```
For each k between i and j:
    Try making triangle (i, k, j):
    dp[i][j] = min(dp[i][j], 
                   dp[i][k] + dp[k][j] + points[i]*points[k]*points[j])
```

**Complexity**: O(n³) time, O(n²) space

---

#### **375. Guess Number Higher or Lower II** (Medium)
**Concept**: Minimize worst-case money spent in guessing game?
- Range [1, n], if guess x: pay x coins, then search left or right
- Find strategy that minimizes maximum cost

**DP State**: `dp[i][j]` = minimum cost for range [i, j]

**Transition**:
```
For each guess k in range [i, j]:
    Cost if k is wrong = k + max(dp[i][k-1], dp[k+1][j])
    dp[i][j] = min(dp[i][j], cost if k is wrong)
```

**Complexity**: O(n³) time, O(n²) space

---

### **Category 3: Subsequence Partitioning**

#### **97. Interleaving String** (Medium)
**Concept**: Can you form string s3 by interleaving s1 and s2?
- s1 = "aab", s2 = "bac", s3 = "aabaaca" → True

**DP State**: `dp[i][j]` = can we form `s3[0...i+j-1]` from `s1[0...i-1]` and `s2[0...j-1]`?

**Transition**:
```
Take from s1: if(s1[i-1] == s3[i+j-1] and dp[i-1][j]) dp[i][j] = true
Take from s2: if(s2[j-1] == s3[i+j-1] and dp[i][j-1]) dp[i][j] = true
```

**Complexity**: O(m*n) time, O(m*n) space

---

#### **87. Scramble String** (Hard)
**Concept**: Can string s1 be transformed into s2 by recursive splits and swaps?
- At each step, split string and either recurse on both halves or swap then recurse

**DP State**: `dp[i1][i2][len]` = can `s1[i1...i1+len-1]` transform into `s2[i2...i2+len-1]`?

**Transition**:
```
For each split point k:
    Option 1 (no swap): 
        dp[i1][i2][len] = dp[i1][i2][k] and dp[i1+k][i2+k][len-k]
    Option 2 (swap):
        dp[i1][i2][len] = dp[i1][i2+len-k][k] and dp[i1+k][i2][len-k]
```

**Complexity**: O(n⁴) time, O(n³) space

---

## Problem Difficulty Progression

| Problem | Difficulty | Pattern | Key Technique |
|---------|-----------|---------|---|
| 91. Decode Ways | Medium | 1D Partition | Linear iteration |
| 139. Word Break | Medium | 1D Partition | Dictionary checking |
| 132. Palindrome Partitioning II | Hard | 1D + Range | Palindrome precompute |
| 97. Interleaving String | Medium | 2D Partition | Subsequence matching |
| 1039. Polygon Triangulation | Medium | Range DP | Interval optimization |
| 375. Guess Number | Medium | Range DP | Minimax strategy |
| 312. Burst Balloons | Hard | Range DP | "Last" instead of "First" |
| 87. Scramble String | Hard | 2D Partition | Recursive structure |

---

## Key Differences from Stock DP

| Aspect | Stock DP | Partition DP |
|--------|----------|-------------|
| **State** | Tracks position + action state (buy/sell) | Tracks range or position |
| **Transition** | Toggle between states | Try all split points |
| **Dimension** | Usually 2D (position × state) | Often 2-3D (ranges or multi-string) |
| **Order** | Time-linear (day by day) | Range-based or interval-based |
| **Complexity** | O(n) or O(n*k) | O(n²) or O(n³) |

---

## Common Mistakes in Partition DP

1. **Wrong base case**: Remember single elements/characters (dp[i][i])
2. **Loop order**: Build from smaller ranges to larger ranges
3. **Split point logic**: Make sure you don't skip or double-count splits
4. **Off-by-one errors**: Careful with indices when combining subproblems
5. **Forgot to memoize**: Always check memo table before recursive calls

---

## Quick Recognition Checklist

Ask yourself:
- [ ] Does the problem ask to split/partition into parts?
- [ ] Are the parts solved independently?
- [ ] Do you need to try all possible split points?
- [ ] Is the solution a combination of subproblem results?
- [ ] Do you need min/max over all possible partitions?

If **ALL YES** → Use Partition DP ✅

---

## Practice Strategy

1. **Start Easy**: Decode Ways (1D linear partition)
2. **Build Confidence**: Word Break (similar but with dictionary)
3. **Add Difficulty**: Palindrome Partitioning II (add palindrome check)
4. **Learn Range DP**: Polygon Triangulation or Guess Number (basic interval)
5. **Master Hard**: Burst Balloons (counterintuitive "last" approach)
6. **Challenge**: Scramble String (complex recursive partitioning)

