//Question: https://leetcode.com/problems/palindrome-partitioning-ii/description/
// Difficulty: Hard | Pattern: 1D Partition + Preprocessing

// Problem: Given string s, find minimum cuts needed to partition s
// such that each part is a palindrome
// Example: s = "nitin" -> 0 cuts (entire string is palindrome)
//          s = "abbb" -> 1 cut: "a" | "bbb"

// Key Insight: Precompute palindromes, then use partition DP

#include <vector>
#include <string>
#include <algorithm>
#include <climits>
using namespace std;

class Solution {
public:
    int minCut(string s) {
        int n = s.length();
        
        // Step 1: Precompute which substrings are palindromes
        // is_pal[i][j] = true if s[i...j] is palindrome
        vector<vector<bool>> is_pal(n, vector<bool>(n, false));
        
        // Every single character is palindrome
        for(int i = 0; i < n; i++) {
            is_pal[i][i] = true;
        }
        
        // Check 2-char palindromes
        for(int i = 0; i < n - 1; i++) {
            if(s[i] == s[i + 1]) {
                is_pal[i][i + 1] = true;
            }
        }
        
        // Check longer palindromes (length >= 3)
        for(int len = 3; len <= n; len++) {
            for(int i = 0; i + len - 1 < n; i++) {
                int j = i + len - 1;
                // s[i...j] is palindrome if s[i] == s[j] AND s[i+1...j-1] is palindrome
                if(s[i] == s[j] && is_pal[i + 1][j - 1]) {
                    is_pal[i][j] = true;
                }
            }
        }
        
        // Step 2: Partition DP
        // dp[i] = minimum cuts needed for s[0...i]
        vector<int> dp(n, INT_MAX);
        
        for(int i = 0; i < n; i++) {
            // If s[0...i] is palindrome, no cuts needed
            if(is_pal[0][i]) {
                dp[i] = 0;
            } else {
                // Try all split points
                for(int j = 0; j < i; j++) {
                    // If s[j+1...i] is palindrome
                    if(is_pal[j + 1][i]) {
                        // Partition: s[0...j] | s[j+1...i]
                        dp[i] = min(dp[i], dp[j] + 1);
                    }
                }
            }
        }
        
        return dp[n - 1];
    }
};

// Complexity: O(n^2) time for palindrome precomputation
//            O(n^2) time for DP
//            O(n^2) space for is_pal table + O(n) for dp array

// Alternative: Expand Around Center for Palindromes
// Can replace the palindrome precomputation step, but overall complexity stays O(n^2)

class Solution_OptimizedSpace {
private:
    vector<vector<bool>> is_pal;
    
    void compute_palindromes(const string& s) {
        int n = s.length();
        is_pal.assign(n, vector<bool>(n, false));
        
        // Single characters
        for(int i = 0; i < n; i++) {
            is_pal[i][i] = true;
        }
        
        // Try all odd-length palindromes (center is single char)
        for(int center = 0; center < n; center++) {
            int left = center, right = center;
            while(left >= 0 && right < n && s[left] == s[right]) {
                is_pal[left][right] = true;
                left--;
                right++;
            }
        }
        
        // Try all even-length palindromes (center between two chars)
        for(int center = 0; center < n - 1; center++) {
            int left = center, right = center + 1;
            while(left >= 0 && right < n && s[left] == s[right]) {
                is_pal[left][right] = true;
                left--;
                right++;
            }
        }
    }
    
public:
    int minCut(string s) {
        int n = s.length();
        compute_palindromes(s);
        
        vector<int> dp(n);
        for(int i = 0; i < n; i++) {
            if(is_pal[0][i]) {
                dp[i] = 0;
            } else {
                dp[i] = INT_MAX;
                for(int j = 0; j < i; j++) {
                    if(is_pal[j + 1][i]) {
                        dp[i] = min(dp[i], dp[j] + 1);
                    }
                }
            }
        }
        
        return dp[n - 1];
    }
};

// Key Learning Points:
// 1. Two-step approach: precompute auxiliary info, then apply partition DP
// 2. Palindrome table is built bottom-up (single char -> longer strings)
// 3. DP state: minimum cuts for prefix s[0...i]
// 4. At each position, try all valid split points where right part is palindrome
// 5. This is 1D partition DP with preprocessing
