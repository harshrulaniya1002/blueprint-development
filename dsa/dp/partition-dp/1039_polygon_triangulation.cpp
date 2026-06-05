//Question: https://leetcode.com/problems/minimum-score-triangulation-of-polygon/
// Difficulty: Medium | Pattern: Range DP (Interval DP)

// Problem: Triangulate a convex polygon to minimize the score
// Score of a triangle = product of its three sides
// Total score = sum of all triangle scores
// Example: points = [1,3,3,4,1,1] (polygon with 6 vertices)
//          One triangulation: triangles (0,1,2), (0,2,3), (0,3,4), (0,4,5)
//          Scores: 1*3*3=9, 1*3*4=12, 1*4*1=4, 1*1*1=1 => Total=26

// Key Insight: dp[i][j] = minimum score to triangulate polygon from vertex i to j

#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class Solution {
public:
    int minScoreTriangulation(vector<int>& values) {
        int n = values.size();
        
        // dp[i][j] = minimum score to triangulate polygon from vertex i to j
        // Only vertices i, i+1, ..., j are involved (not the entire polygon between them)
        vector<vector<int>> dp(n, vector<int>(n, 0));
        
        // Build up by increasing the distance (j - i)
        for(int len = 2; len < n; len++) {
            for(int i = 0; i + len < n; i++) {
                int j = i + len;
                dp[i][j] = INT_MAX;
                
                // Try each vertex k between i and j as the third vertex
                // of the triangle (i, k, j)
                for(int k = i + 1; k < j; k++) {
                    // Make triangle (i, k, j)
                    // Score = values[i] * values[k] * values[j]
                    // Plus score from triangulating (i, k) and (k, j)
                    int score = values[i] * values[k] * values[j] 
                              + dp[i][k] + dp[k][j];
                    dp[i][j] = min(dp[i][j], score);
                }
            }
        }
        
        // Return triangulation of entire polygon (vertices 0 to n-1)
        // But we need to account for the final outer triangle
        return dp[0][n - 1];
    }
};

// Complexity: O(n^3) time, O(n^2) space

// Example trace: values = [1, 2, 3, 4] (4-gon/quadrilateral)
//
// Base case: len = 2
//   dp[0][2]: Can't triangulate (need 3 distinct vertices)
//   dp[1][3]: Can't triangulate
//   (These remain 0 or are not used)
//
// len = 3: (Full problem for quadrilateral)
//   i=0, j=3: Try k=1,2
//     k=1: Triangle (0,1,3), score = 1*2*4 + dp[0][1] + dp[1][3]
//          dp[0][1] = 0, dp[1][3] = need to compute
//          But dp[1][3] has length 2, which we haven't computed properly
//          Actually for len=3, we compute:
//            k=1: score = 1*2*4 + 0 + dp[1][3]
//                 dp[1][3] not computed for len=2
//     k=2: Triangle (0,2,3), score = 1*3*4 + dp[0][2] + dp[2][3]
//
// Wait, let me reconsider the length definition...
//
// Actually, len = j - i should work:
// len=0: single vertex (not used)
// len=1: two vertices (not used)
// len=2: three vertices (i, i+1, i+2)
//
// dp[0][2]: k can be 1
//   Triangle (0,1,2): score = 1*2*3 = 6
// dp[1][3]: k can be 2
//   Triangle (1,2,3): score = 2*3*4 = 24
//
// dp[0][3]: k can be 1 or 2
//   k=1: Triangle (0,1,3): 1*2*4 + dp[0][1] + dp[1][3]
//        dp[0][1] = 0 (can't triangulate 2 vertices)
//        dp[1][3] = 24
//        Total = 8 + 0 + 24 = 32
//   k=2: Triangle (0,2,3): 1*3*4 + dp[0][2] + dp[2][3]
//        dp[0][2] = 6
//        dp[2][3] = 0
//        Total = 12 + 6 + 0 = 18
//   Answer: min(32, 18) = 18

// Why this works:
// When we triangulate polygon i to j:
// - We pick one triangle (i, k, j) to "commit to"
// - This triangle separates the polygon into 3 regions:
//   1. Triangle (i, k, j) itself with score values[i]*values[k]*values[j]
//   2. Subpolygon from i to k (vertices i to k)
//   3. Subpolygon from k to j (vertices k to j)
// - Regions 2 and 3 are triangulated independently
// - Total = score of main triangle + optimal triangulations of subregions

// This is classic Range DP (Interval DP) pattern
