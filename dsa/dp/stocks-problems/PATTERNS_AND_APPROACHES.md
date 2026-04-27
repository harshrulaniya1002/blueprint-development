# Stock Trading Problems - Patterns & Approaches Guide

Quick reference guide for the 6 types of stock trading problems and their solutions.

---

## Problem Overview

| Type | Problem | Constraint | Approach | Complexity |
|------|---------|-----------|----------|-----------|
| 1 | Buy & Sell Stock | At most 1 transaction | Greedy | O(n) time, O(1) space |
| 2 | Buy & Sell Stock II | Unlimited transactions | DP with States | O(n) time, O(n) space |
| 3 | Buy & Sell Stock III | At most 2 transactions | DP with States + Cap | O(n) time, O(n) space |
| 4 | Buy & Sell Stock IV | At most k transactions | DP with States + Cap | O(n*k) time, O(n*k) space |
| 5 | Buy & Sell Stock + Cooldown | Unlimited + cooldown | DP with Skip | O(n) time, O(n) space |
| 6 | Buy & Sell Stock + Fee | Unlimited + fee | DP with Fee | O(n) time, O(n) space |

---

## Type 1: Single Transaction (Greedy)

**Problem**: [Best Time to Buy and Sell Stock](https://leetcode.com/problems/best-time-to-buy-and-sell-stock/)

**Key Insight**: You can only buy once and sell once. Find the minimum price seen so far and maximize the profit.

**Pattern**: **Single Pass with Tracking Minimum**

**Approach**:
- Track the minimum price encountered so far
- At each index, calculate profit if you sell at current price
- Keep track of maximum profit seen

**Code Structure**:
```cpp
int mini = INT_MAX;
int maxProfit = 0;
for each price:
    mini = min(mini, price)
    maxProfit = max(maxProfit, price - mini)
return maxProfit
```

**Why It Works**: 
- Since you need one buy and one sell, you want to buy at the lowest point before selling
- By tracking minimum as you iterate, you ensure you buy before you sell
- No DP needed due to single transaction constraint

**Complexity**: O(n) time, O(1) space

---

## Type 2: Unlimited Transactions

**Problem**: [Best Time to Buy and Sell Stock II](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-ii/)

**Key Insight**: You can buy and sell as many times as you want. The optimal strategy is to capture every upward swing.

**Pattern**: **DP with Buy/Sell State**

**State Definition**:
- `dp[i][buy]` = maximum profit at index i
- `buy = 1` means we're currently holding a stock
- `buy = 0` means we're not holding any stock

**Transitions**:
```
If buy == 1 (holding stock):
    - Option 1: Sell now and move to next day → arr[i] + func(i+1, 0)
    - Option 2: Do nothing (skip) → func(i+1, 1)
    - Take maximum

If buy == 0 (not holding):
    - Option 1: Buy now and move to next day → -arr[i] + func(i+1, 1)
    - Option 2: Do nothing (skip) → func(i+1, 0)
    - Take maximum
```

**Why It Works**:
- At each day, we have choices based on current state (holding or not)
- We track both states simultaneously using DP
- The state toggles between buy and sell

**Complexity**: O(n) time, O(n) space

---

## Type 3: At Most 2 Transactions

**Problem**: [Best Time to Buy and Sell Stock III](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iii/)

**Key Insight**: Limited to 2 complete transactions. Need to track how many transactions are left.

**Pattern**: **DP with Buy/Sell State + Transaction Capacity**

**State Definition**:
- `dp[i][buy][cap]` = maximum profit at index i
- `buy = 1` means currently holding stock
- `cap` = number of transactions left (1 or 2)

**Note**: A transaction = BUY + SELL (selling decreases capacity)

**Transitions**:
```
If buy == 1 (holding):
    - Option 1: Sell → arr[i] + func(i+1, 0, cap-1)  // capacity decreases
    - Option 2: Do nothing → func(i+1, 1, cap)
    
If buy == 0 (not holding):
    - Option 1: Buy → -arr[i] + func(i+1, 1, cap)  // capacity unchanged
    - Option 2: Do nothing → func(i+1, 0, cap)
```

**Why It Works**:
- The capacity constraint is enforced by only decrementing cap on SELL
- Buying doesn't consume capacity, only selling does
- This ensures we can have at most 2 complete buy-sell pairs

**Complexity**: O(n) time, O(2n) space

---

## Type 4: At Most k Transactions

**Problem**: [Best Time to Buy and Sell Stock IV](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iv/)

**Key Insight**: Generalized version of Type 3. Can have at most k transactions.

**Pattern**: **DP with Buy/Sell State + Variable Capacity**

**State Definition**:
- `dp[i][buy][cap]` = maximum profit at index i
- `buy = 1` means currently holding stock
- `cap` = number of transactions left (can be 1 to k)

**Transitions**: Same as Type 3, but cap goes from 1 to k

**Key Optimization**:
- If `k >= n/2`, you can do unlimited transactions (same as Type 2)
- This prevents unnecessary DP computation for large k values

**Complexity**: O(n*k) time, O(n*k) space

---

## Type 5: Unlimited Transactions + Cooldown

**Problem**: [Best Time to Buy and Sell Stock with Cooldown](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-cooldown/)

**Key Insight**: After selling, you must wait 1 day before buying again (cooldown).

**Pattern**: **DP with Buy/Sell State + Skip on Sell**

**State Definition**:
- `dp[i][buy]` = maximum profit at index i
- `buy = 1` means currently holding stock
- `buy = 0` means not holding

**Critical Difference**:
```
If buy == 0 (not holding) and you SELL:
    - SKIP to i+2, not i+1
    → arr[i] + func(i+2, 1)  // Skip i+1 for cooldown
    
If buy == 0 (not holding) and you DON'T SELL:
    - Move to i+1
    → func(i+1, 0)
```

**Why It Works**:
- When you sell, the `i+2` ensures you can't buy on the next day (cooldown period)
- The DP still considers all valid states but respects the cooldown constraint

**Complexity**: O(n) time, O(n) space

---

## Type 6: Unlimited Transactions + Transaction Fee

**Problem**: [Best Time to Buy and Sell Stock with Transaction Fee](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-transaction-fee/)

**Key Insight**: Each transaction (buying or selling) incurs a fee. Optimize profit after paying fees.

**Pattern**: **DP with Buy/Sell State + Fee Deduction**

**State Definition**:
- `dp[i][buy]` = maximum profit at index i
- `buy = 1` means currently holding stock

**Critical Difference**:
```
If buy == 0 (not holding) and you SELL:
    - Subtract fee from profit
    → arr[i] + func(i+1, 1) - Fee
    
If buy == 1 (holding) and you BUY:
    - Can include fee here too (implementation choice)
    → -arr[i] + func(i+1, 0) - Fee (optional)
```

**Note**: Fee is typically applied once per transaction (either on buy or sell, not both).

**Why It Works**:
- The fee reduces profit on each transaction
- DP still finds optimal sequence, just with adjusted profitability
- The state transitions remain the same as Type 2

**Complexity**: O(n) time, O(n) space

---

## Comparison Table: Key Differences

| Feature | Type 1 | Type 2 | Type 3 | Type 4 | Type 5 | Type 6 |
|---------|--------|--------|--------|--------|--------|--------|
| Algorithm | Greedy | DP | DP | DP | DP | DP |
| State Count | — | 2 | 6 | 2k | 2 | 2 |
| Extra Constraint | None | Unlimited | 2 txns max | k txns max | Cooldown | Fee |
| Special Logic | Min tracking | Toggle state | Cap decrement | Cap decrement | Skip on sell | Fee subtract |
| Space Optimized | Yes (O(1)) | Yes (O(n)) | Yes (O(n)) | Yes (O(n*k)) | Yes (O(n)) | Yes (O(n)) |

---

## DP Template Pattern

All DP solutions follow this general structure:

```cpp
int func(int i, int buy, int extra_param) {
    // Base case: reached end of prices
    if(i >= n) return 0;
    
    // Base case: constraint reached
    if(extra_param == 0) return 0;
    
    // Check memoization
    if(dp[i][buy][extra_param] != -1) 
        return dp[i][buy][extra_param];
    
    int result;
    if(buy == 1) {
        // State: currently holding stock
        // Option 1: Sell now
        int sell = arr[i] + func(i+next, 0, new_cap);
        // Option 2: Skip
        int skip = func(i+1, buy, extra_param);
        result = max(sell, skip);
    } else {
        // State: not holding stock
        // Option 1: Buy now
        int buy_now = -arr[i] + func(i+1, 1, extra_param);
        // Option 2: Skip
        int skip = func(i+1, buy, extra_param);
        result = max(buy_now, skip);
    }
    
    return dp[i][buy][extra_param] = result;
}
```

---

## Quick Revision Checklist

- [ ] **Type 1**: Greedy with min tracking, no DP needed
- [ ] **Type 2**: Basic DP with buy/sell toggle
- [ ] **Type 3**: Type 2 + capacity constraint (decrement on sell only)
- [ ] **Type 4**: Type 3 but with variable k instead of fixed 2
- [ ] **Type 5**: Type 2 but skip to `i+2` after selling (cooldown)
- [ ] **Type 6**: Type 2 but subtract fee on sell

---

## Common Mistakes to Avoid

1. **Type 3 & 4**: Decrementing cap on BUY instead of SELL
2. **Type 5**: Forgetting to skip to `i+2` when selling
3. **Type 6**: Subtracting fee from both buy and sell (do it once)
4. **All DP**: Forgetting to initialize DP array with -1
5. **All DP**: Off-by-one errors in array bounds check

---

## Practice Strategy

1. Start with **Type 1** (Greedy intuition)
2. Move to **Type 2** (Basic DP template)
3. Learn **Type 3** (Adding constraints to DP)
4. Understand **Type 4** (Generalizing constraints)
5. Master **Type 5** (Modifying state transitions)
6. Perfect **Type 6** (Applying costs in DP)

---

## Related Problems by Pattern

Learning these stock problems gives you a toolkit to solve many other LeetCode problems. Here's how they map:

### 1. State Machine & DP (Similar to "Cooldown" or "K-Transactions")

These problems require you to track **states** (e.g., holding vs. not holding) to decide your next move. The DP structure is identical to Type 5 and Type 4.

#### **198. House Robber** (Easy/Medium)
**Link**: https://leetcode.com/problems/house-robber/

**Connection**: Just like you can't buy stock on a "cooldown" day, you can't rob two adjacent houses.

**Pattern**: Track two choices at each house:
- `rob_this_house`: profit from robbing this house + best result from `i-2`
- `skip_this_house`: best result from `i-1`

**Why It's Related**: State machine pattern (rob or skip), same as stock buy/sell.

---

#### **213. House Robber II** (Medium)
**Link**: https://leetcode.com/problems/house-robber-ii/

**Connection**: Adds a **circular constraint**. Similar to how "Stock III" limits transactions, this breaks the problem into two sub-problems to handle the boundary.

**Pattern**: 
- Cannot rob both house 0 and house n-1
- Solution = max(rob houses 0 to n-2, rob houses 1 to n-1)

**Why It's Related**: Handling constraints by splitting into independent sub-problems (like handling k-transactions).

---

#### **276. Paint Fence** (Medium)
**Link**: https://leetcode.com/problems/paint-fence/

**Connection**: Similar to Type 5 (Cooldown). You decide to paint a post either the "same" color as the last one or a "different" color, with constraints on consecutive actions.

**Pattern**:
- `same`: paint same color as post i-1 (only valid if i-1 was painted "different")
- `different`: paint different color than post i-1

**Why It's Related**: State transitions with constraints based on previous state.

---

### 2. Greedy "Local Peaks" (Similar to "Stock II")

If a problem allows you to make an **infinite number of choices** to maximize a sum, it's often a Greedy "Stock II" variation: capture every upward swing.

#### **134. Gas Station** (Medium)
**Link**: https://leetcode.com/problems/gas-station/

**Connection**: You are looking for a starting point that maintains a "positive balance" throughout a cycle. Much like tracking price dips to buy, you track "gas deficits" to find the right start.

**Pattern**: 
- Keep a running balance of gas - cost
- If balance goes negative, reset and try next station
- Track total balance: if positive, answer exists

**Why It's Related**: Greedy scanning with state tracking (similar to finding the minimum in Type 1).

---

#### **452. Minimum Number of Arrows to Burst Balloons** (Medium)
**Link**: https://leetcode.com/problems/minimum-number-of-arrows-to-burst-balloons/

**Connection**: Uses interval logic. You "sell" (pop) at the earliest possible end-point to maximize "profit" (balloons popped) per arrow.

**Pattern**:
- Sort by end point (like finding best exit)
- Greedily select intervals that don't overlap
- Each arrow bursts as many balloons as possible

**Why It's Related**: Greedy selection of "peaks" (overlapping intervals) similar to capturing all upswings in Stock II.

---

### 3. Difference Arrays / Post-Processing (Similar to "Stock I")

If you can solve a problem by looking at the **difference between two points**, these apply. Stock I is fundamentally finding `max(prices[j] - prices[i])`.

#### **53. Maximum Subarray** (Medium) — Kadane's Algorithm
**Link**: https://leetcode.com/problems/maximum-subarray/

**Connection**: This is the "Stock I" logic in disguise! In Stock I, you find $\max(price_j - price_i)$. If you convert prices into an array of daily changes, finding max profit is exactly the same as finding the Maximum Subarray Sum.

**Pattern**:
```
If prices = [7, 1, 5, 3, 6, 4]
Differences = [-6, 4, -2, 3, -2]
Max subarray of differences = 4 = max profit (buy at 1, sell at 5)
```

**Why It's Related**: Same mathematical foundation—finding maximum difference over a range.

---

#### **11. Container With Most Water** (Medium)
**Link**: https://leetcode.com/problems/container-with-most-water/

**Connection**: Similar to finding the "lowest buy" and "highest sell," you use **two pointers** to find the boundaries that maximize area.

**Pattern**:
- Start with leftmost and rightmost heights
- Water trapped = `min(left, right) × distance`
- Move the pointer with smaller height inward (greedy)

**Why It's Related**: Boundary optimization and greedy pointer movement (similar to Stock I's "min price tracking").

---

### 4. Advanced: Slope Trick / Monotonic Queue (Hardest)

For the hardest versions (like Stock IV), these problems share the same mathematical "optimization" DNA: managing structural constraints with efficient data structures.

#### **84. Largest Rectangle in Histogram** (Hard)
**Link**: https://leetcode.com/problems/largest-rectangle-in-histogram/

**Connection**: Uses a **Monotonic Stack** to find the "best buy/sell" boundaries for height. Similar to Stock IV's DP optimization.

**Pattern**:
- Use stack to maintain increasing heights
- When a smaller height is found, pop and calculate area
- This finds the best left/right boundaries for each bar

**Why It's Related**: Optimization technique for finding "best boundaries" under constraints (similar to Stock IV's transactional logic).

---

#### **407. Trapping Rain Water II** (Hard)
**Link**: https://leetcode.com/problems/trapping-rain-water-ii/

**Connection**: While 2D, it uses the "min-max" boundary logic to determine how much "profit" (water) each cell can hold. Similar to Stock's profit = (sell price - buy price).

**Pattern**:
- Use min-heap to process cells from boundary inward
- Water level = max(previous level, current cell height)
- Profit (water) = water level - cell height

**Why It's Related**: Boundary-based profit calculation in higher dimensions.

---

## Related Problems Reference Table

| Problem | Difficulty | Similar Type | Key Concept |
|---------|------------|--------------|------------|
| 198. House Robber | Easy | Type 2-5 | State machine DP |
| 213. House Robber II | Medium | Type 3-4 | Constraint handling |
| 276. Paint Fence | Medium | Type 5 | State transitions |
| 134. Gas Station | Medium | Type 2 | Greedy scanning |
| 452. Balloons | Medium | Type 2 | Greedy selection |
| 53. Max Subarray | Medium | Type 1 | Difference arrays |
| 11. Container Water | Medium | Type 1 | Two pointers |
| 84. Largest Rectangle | Hard | Type 4 | Monotonic structure |
| 407. Rain Water II | Hard | Type 4 | Min-max boundaries |

---

## How to Use This Section

1. **Feeling comfortable with Type 1-2?** Try **House Robber** and **Gas Station**
2. **Mastered Type 3-4?** Tackle **House Robber II** and **Paint Fence**
3. **Want to solidify greedy logic?** Practice **Container With Most Water** and **Balloons**
4. **Ready for Hard?** Attempt **Largest Rectangle in Histogram** and **Trapping Rain Water II**

