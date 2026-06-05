//Question: https://leetcode.com/problems/guess-number-higher-or-lower-ii/
// Difficulty: Medium | Pattern: Range DP with Minimax

// Problem: In a guessing game where you guess a number in range [1, n]
// If you guess x (wrong), you pay x coins
// Then the answer is either in [1, x-1] or [x+1, n]
// Find the minimum worst-case cost to guarantee winning

// Example: n = 10
// If you guess 6 first and it's wrong:
//   Worst case: answer is in [7,10]
//   Then guess 8: if wrong, answer in [9,10]
//   Then guess 9: if wrong, answer is 10
// Worst case path: 6 + 8 + 9 = 23
// But better strategy exists!

// Key Insight: dp[i][j] = minimum worst-case cost for range [i, j]

#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class Solution {
public:
    int getMoneyAmount(int n) {
        // dp[i][j] = minimum worst-case cost to win in range [i, j]
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
        
        // Build up by increasing range length
        for(int len = 2; len <= n; len++) {
            for(int i = 1; i + len - 1 <= n; i++) {
                int j = i + len - 1;
                dp[i][j] = INT_MAX;
                
                // Try each guess k in range [i, j]
                for(int k = i; k < j; k++) {
                    // If we guess k and it's wrong:
                    // - We pay k coins
                    // - If answer is < k: recurse on [i, k-1]
                    // - If answer is > k: recurse on [k+1, j]
                    // Worst case = max of two subproblems
                    // Total cost = k + max(left_cost, right_cost)
                    
                    int cost_if_wrong = k + max(dp[i][k-1], dp[k+1][j]);
                    dp[i][j] = min(dp[i][j], cost_if_wrong);
                }
            }
        }
        
        return dp[1][n];
    }
};

// Complexity: O(n^3) time, O(n^2) space

// Example trace: n = 10
// 
// Base cases: len = 1 (single number)
// dp[i][i] = 0 (if range is single number, we know it immediately, pay 0)
//
// len = 2: ranges of size 2 like [1,2], [2,3], etc.
// dp[1][2]: range [1,2]
//   Try k=1: cost = 1 + max(dp[1][0], dp[2][2]) = 1 + max(0, 0) = 1
//   Answer: dp[1][2] = 1
//   Strategy: Always guess lower number first
//
// dp[1][3]: range [1,3]
//   Try k=1: cost = 1 + max(0, dp[2][3]) = 1 + dp[2][3]
//   Try k=2: cost = 2 + max(dp[1][1], dp[3][3]) = 2 + max(0, 0) = 2
//   First need dp[2][3] = 2 (from len=2 case)
//   k=1: 1 + 2 = 3
//   k=2: 2 + 0 = 2
//   Answer: dp[1][3] = 2 (guess 2, if wrong narrow to [1,1] or [3,3])
//
// This continues for all ranges...

// Key insight (Minimax strategy):
// - We're choosing the WORST-case scenario for ourselves (minimize worst-case cost)
// - max(left, right) represents the worst case after our guess
// - We want to minimize this worst case
// - This is the Minimax principle from game theory

// Why "k < j" in the loop?
// If we guess k in range [i, j]:
//   - If answer is < k: search [i, k-1]
//   - If answer is > k: search [k+1, j]
//   - If answer is k: we win!
// So we never need to try k=j because if answer is >= j, the last guess doesn't help

// Relation to Stock DP:
// - Stock DP: "what state am I in?" tracking
// - This DP: "what's my optimal strategy?" (minimax over choices)
// - Both use ranges and subproblems, but different reasoning
