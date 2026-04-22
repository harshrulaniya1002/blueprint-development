// Problem: Unrooted tree, each node has a price.
// For a chosen root r, cost(r) = max_path_sum - min_path_sum over all paths starting at r.
// min_path_sum from r is always price[r] (trivial single-node path).
// So cost(r) = maxPath(r) - price[r].
// Return max cost over all root choices.
//
// Technique: Tree Rerooting DP  (NOT LCA)
// Why NOT LCA: LCA is for arbitrary node-to-node path queries.
//              Here all paths share a common endpoint (the root), so rerooting DP suffices.
//
// Time: O(n), Space: O(n)

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long maxOutput(int n, vector<vector<int>>& edges, vector<int>& price) {
        vector<vector<int>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        vector<long long> down(n, 0); // max path sum going DOWN from v into its subtree

        // ---- DFS 1: compute down[] (root tree at node 0) ----
        // down[v] = price[v] + max(0, max over children c of down[c])
        function<void(int, int)> dfs1 = [&](int u, int par) {
            down[u] = price[u];
            for (int v : adj[u]) {
                if (v == par) continue;
                dfs1(v, u);
                down[u] = max(down[u], price[u] + down[v]);
            }
        };
        dfs1(0, -1);

        long long ans = 0;

        // ---- DFS 2: rerooting ----
        // upVal = best path sum starting at u going UPWARD (already includes price[u])
        // For node 0 (root), upVal = price[0] with no upward contribution, but to keep
        // the formula uniform we pass 0 and add price[u] inside.
        //
        // upFromParent = max path sum if we leave u via its parent direction
        //                (this is what the parent computed as the best path not through u,
        //                 plus price[u] added here)

        function<void(int, int, long long)> dfs2 = [&](int u, int par, long long upFromParent) {
            // maxPath[u] = max(down[u], upFromParent)
            long long maxPath = max(down[u], upFromParent);
            ans = max(ans, maxPath - price[u]);

            // For each child v, we need: best path from u NOT going through v
            // = max(upFromParent, max over other children c≠v of down[c]) + price[u]
            //
            // Precompute top-2 down[] values among children to handle "best sibling" in O(1)
            long long best1 = upFromParent, best2 = LLONG_MIN;
            int best1_child = -1;

            for (int v : adj[u]) {
                if (v == par) continue;
                if (down[v] > best1) {
                    best2 = best1;
                    best1 = down[v];
                    best1_child = v;
                } else if (down[v] > best2) {
                    best2 = down[v];
                }
            }

            for (int v : adj[u]) {
                if (v == par) continue;
                // Best path from u not going through v:
                long long bestOther = (v == best1_child) ? best2 : best1;
                // bestOther could be LLONG_MIN if no valid option; use max with upFromParent
                // Actually best1 is initialized to upFromParent, so bestOther >= upFromParent
                // when best1_child != v; when best1_child == v, bestOther = best2 which may
                // be LLONG_MIN if there are no siblings. Guard:
                long long upForChild;
                if (bestOther == LLONG_MIN) {
                    // u has only one child (v) and upFromParent was beaten
                    upForChild = price[u] + upFromParent;
                } else {
                    upForChild = price[u] + bestOther;
                }
                dfs2(v, u, upForChild);
            }
        };

        dfs2(0, -1, 0);
        return ans;
    }
};

// ---- Test ----
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Example 1: n=6, edges=[[0,1],[1,2],[1,3],[3,4],[3,5]], price=[9,8,1,6,4,8]
    // Expected output: 24
    {
        int n = 6;
        vector<vector<int>> edges = {{0,1},{1,2},{1,3},{3,4},{3,5}};
        vector<int> price = {9,8,1,6,4,8};
        Solution sol;
        cout << "Test 1: " << sol.maxOutput(n, edges, price) << " (expected 24)\n";
    }

    // Example 2: n=3, edges=[[0,1],[1,2]], price=[1,1,1]
    // Expected output: 2
    {
        int n = 3;
        vector<vector<int>> edges = {{0,1},{1,2}};
        vector<int> price = {1,1,1};
        Solution sol;
        cout << "Test 2: " << sol.maxOutput(n, edges, price) << " (expected 2)\n";
    }

    return 0;
}
