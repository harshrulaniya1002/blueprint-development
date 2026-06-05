//Question: https://leetcode.com/problems/burst-balloons/description/
// Difficulty: Hard | Pattern: Range DP (Interval DP)

// Problem: Burst balloons to maximize coins
// When you burst balloon i, adjacent balloons become neighbors
// Coins from burst = nums[left] * nums[i] * nums[right]
// Example: nums = [3,1,5,8]
//          Burst order matters! If we burst 1 (index 1) last:
//          We get 3 * 1 * 5 = 15 coins

// Key Insight (CRITICAL): Think "which balloon to burst LAST" NOT first
// This solves the dependency problem of adjacent balloons changing

#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class Solution {
public:
    int maxCoins(vector<int>& nums) {
        int n = nums.size();
        
        // Add padding: 1 at both ends ensures boundaries always exist
        vector<int> balloons(n + 2);
        balloons[0] = 1;
        balloons[n + 1] = 1;
        for(int i = 0; i < n; i++) {
            balloons[i + 1] = nums[i];
        }
        
        // dp[i][j] = max coins from bursting balloons between i and j (exclusive)
        // i.e., balloons[i+1...j-1] only
        vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));
        
        // Build up by increasing interval length
        for(int len = 2; len <= n + 1; len++) {
            for(int i = 0; i + len - 1 <= n + 1; i++) {
                int j = i + len - 1;
                
                // Try bursting each balloon k LAST in range (i, j)
                for(int k = i + 1; k < j; k++) {
                    // Coins from bursting k = balloons[i] * balloons[k] * balloons[j]
                    // Plus coins from left and right subproblems
                    int coins = balloons[i] * balloons[k] * balloons[j] 
                              + dp[i][k] + dp[k][j];
                    dp[i][j] = max(dp[i][j], coins);
                }
            }
        }
        
        return dp[0][n + 1];
    }
};

// Complexity: O(n^3) time, O(n^2) space

// Why "burst LAST" instead of "burst FIRST"?
// 
// If we try "burst first":
//   dp[i][j] = max over k in (i,j) of:
//              coins[i]*balloons[k]*balloons[j] + dp[i][k] + dp[k][j]
//   PROBLEM: When we burst k first, balloons[i] and balloons[j] become neighbors
//            But we already accounted for balloons[i] in dp[i][k]'s computation
//            This causes inconsistency!
//
// If we think "burst LAST":
//   When k is burst last in range (i,j), ALL balloons between i and j are gone
//   So k's neighbors are exactly balloons[i] and balloons[j] at burst time
//   This is consistent! No overlap in calculations

// Example trace: nums = [3,1,5,8]
// balloons = [1, 3, 1, 5, 8, 1]
// indices:    0  1  2  3  4  5
//
// We want dp[0][5] = max coins from bursting balloons[1..4] = {3,1,5,8}
// Try bursting each as LAST:
//   k=1 (balloon 3 bursts last): 1*3*1 + dp[0][1] + dp[1][5]
//       dp[0][1] = 0 (no balloons between 0 and 1)
//       dp[1][5] = max coins from {1,5,8} when 1 is already burst
//   k=2 (balloon 1 bursts last): 1*1*1 + dp[0][2] + dp[2][5]
//   k=3 (balloon 5 bursts last): 1*5*1 + dp[0][3] + dp[3][5]
//   k=4 (balloon 8 bursts last): 1*8*1 + dp[0][4] + dp[4][5]

// Why padding with 1's?
// - Prevents boundary checks
// - The padding balloons (1's) never get burst
// - Acts as "walls" for the subproblems
// - All legitimate coins calculations remain the same
