//Question: https://leetcode.com/problems/decode-ways/description/
// Difficulty: Medium | Pattern: 1D Partition

// Problem: Given a string with digits, count ways to decode it
// Example: "12" -> ["1,2", "12"] = 2 ways
//          "226" -> ["2,2,6", "2,26", "22,6"] = 3 ways

// Key Insight: At each position, decide whether to take 1 digit or 2 digits

#include <vector>
#include <string>
using namespace std;

class Solution {
public:
    int numDecodings(string s) {
        int n = s.length();
        if(n == 0 || s[0] == '0') return 0;
        
        vector<int> dp(n + 1);
        dp[0] = 1;  // empty string
        dp[1] = 1;  // first character (we know s[0] != '0')
        
        for(int i = 2; i <= n; i++) {
            // Option 1: Take 1 digit (current digit)
            int one_digit = s[i-1] - '0';
            if(one_digit >= 1 && one_digit <= 9) {
                dp[i] += dp[i-1];
            }
            
            // Option 2: Take 2 digits (previous + current)
            int two_digits = (s[i-2] - '0') * 10 + (s[i-1] - '0');
            if(two_digits >= 10 && two_digits <= 26) {
                dp[i] += dp[i-2];
            }
        }
        
        return dp[n];
    }
};

// Complexity: O(n) time, O(n) space
// Space optimized: O(1) space (only need prev and curr)

class Solution_Optimized {
public:
    int numDecodings(string s) {
        int n = s.length();
        if(n == 0 || s[0] == '0') return 0;
        
        int prev2 = 1;  // dp[i-2]
        int prev1 = 1;  // dp[i-1]
        
        for(int i = 2; i <= n; i++) {
            int curr = 0;
            
            // Take 1 digit
            int one_digit = s[i-1] - '0';
            if(one_digit >= 1 && one_digit <= 9) {
                curr += prev1;
            }
            
            // Take 2 digits
            int two_digits = (s[i-2] - '0') * 10 + (s[i-1] - '0');
            if(two_digits >= 10 && two_digits <= 26) {
                curr += prev2;
            }
            
            prev2 = prev1;
            prev1 = curr;
        }
        
        return prev1;
    }
};

// Relation to Stock DP:
// - Stock DP tracks state (buy/sell) at each day
// - Decode DP tries different "partitions" (1 or 2 digits) at each position
// - Both are linear-time DP but with different thinking models
