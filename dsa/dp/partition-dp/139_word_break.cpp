//Question: https://leetcode.com/problems/word-break/description/
// Difficulty: Medium | Pattern: 1D Partition

// Problem: Given string s and word dict, can s be segmented into dict words?
// Example: s = "catsandcatsdog", dict = {"cat","cats","and","dog","sand","cat","catscat"}
//          Can partition as "cats|and|cats|dog" -> True

// Key Insight: At each position, check if any prefix is a valid dictionary word

#include <vector>
#include <string>
#include <unordered_set>
using namespace std;

class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        int n = s.length();
        
        // Convert vector to set for O(1) lookup
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        
        // dp[i] = can s[0...i-1] be segmented into dict words?
        vector<bool> dp(n + 1, false);
        dp[0] = true;  // empty string
        
        for(int i = 1; i <= n; i++) {
            // Try all split points from 0 to i
            for(int j = 0; j < i; j++) {
                // If s[0...j-1] can be segmented (dp[j] == true)
                // AND s[j...i-1] is in dictionary
                if(dp[j] && dict.count(s.substr(j, i - j))) {
                    dp[i] = true;
                    break;  // Found a valid partition
                }
            }
        }
        
        return dp[n];
    }
};

// Complexity: O(n^2) time (two loops), O(n + m) space
//            where m = total chars in all dictionary words

// Optimization with Trie:
#include <memory>

struct TrieNode {
    unordered_map<char, shared_ptr<TrieNode>> children;
    bool isWord = false;
};

class Solution_Trie {
private:
    shared_ptr<TrieNode> root;
    
    void insert(string& word) {
        auto node = root;
        for(char c : word) {
            if(!node->children[c]) {
                node->children[c] = make_shared<TrieNode>();
            }
            node = node->children[c];
        }
        node->isWord = true;
    }
    
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        // Build trie
        root = make_shared<TrieNode>();
        for(auto& word : wordDict) {
            insert(word);
        }
        
        int n = s.length();
        vector<bool> dp(n + 1, false);
        dp[0] = true;
        
        for(int i = 1; i <= n; i++) {
            if(!dp[i]) {
                // Try to extend from previous valid positions
                auto node = root;
                for(int j = i - 1; j >= 0; j--) {
                    if(!dp[j]) break;
                    
                    if(!node->children[s[j]]) break;
                    node = node->children[s[j]];
                    
                    if(node->isWord && dp[j]) {
                        dp[i] = true;
                        break;
                    }
                }
            }
        }
        
        return dp[n];
    }
};

// Trie version: O(n^2) worst case, but better average case
//               O(total_chars_in_dict) space for trie

// Relation to Partition DP:
// At each position i, we partition the string as:
//   s[0...j-1] (already valid) | s[j...i-1] (must be in dict)
// We try ALL possible split points j and pick any that work
// This is the core partition DP thinking
