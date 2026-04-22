#include <bits/stdc++.h>
using namespace std;


// What is max_jump[node][j]?It is a "summary" of a path segment.Definition:
// It stores the maximum node value encountered while jumping from node upwards to its $2^j$-th ancestor.
// Analogy: If parent[node][j] is your destination after a jump, max_jump[node][j] is the highest mountain you flew over during that specific jump.
// Why Binary Lifting works for MaxWe use the DP (Dynamic Programming) property. 
// To find the max value over a jump of $2^j$, we split it into two jumps of $2^{j-1}$.Jump from node to a midpoint ($2^{j-1}$ steps).
// Jump from that midpoint to the final ancestor ($another\ 2^{j-1}$ steps).max_jump[node][j] = max(max_of_first_half, max_of_second_half).


const int MAX_LOG = 18;

void dfs(int node, int par, const vector<vector<int>>& adj, 
         vector<int>& depth, vector<vector<int>>& parent, 
         vector<vector<int>>& max_jump, const vector<int>& a) {
    
    depth[node] = depth[par] + 1;
    parent[node][0] = par;
    // Base case: the max value jumping 2^0 steps is just the value of the current node
    max_jump[node][0] = a[node - 1]; 

    for (int j = 1; j < MAX_LOG; j++) {
        parent[node][j] = parent[parent[node][j - 1]][j - 1];
        // The max over 2^j steps is the max of the two 2^(j-1) jumps
        max_jump[node][j] = max(max_jump[node][j - 1], max_jump[parent[node][j - 1]][j - 1]);
    }

    for (int neighbor : adj[node]) {
        if (neighbor != par) {
            dfs(neighbor, node, adj, depth, parent, max_jump, a);
        }
    }
}


// Query to find maximum value on path between u and v
// using Binary Lifting with max_jump
int query_max(int u, int v, const vector<int>& depth, 
              const vector<vector<int>>& parent, 
              const vector<vector<int>>& max_jump, const vector<int>& a) {
    
    int res = max(a[u - 1], a[v - 1]); // Initialize with endpoint values
    if (depth[u] < depth[v]) swap(u, v);

    // 1. Lift u to same depth as v
    int diff = depth[u] - depth[v];
    for (int j = 0; j < MAX_LOG; j++) {
        if ((diff >> j) & 1) {
            res = max(res, max_jump[u][j]);
            u = parent[u][j];
        }
    }

    if (u == v) return res;

    // 2. Lift both until just below LCA
    for (int j = MAX_LOG - 1; j >= 0; j--) {
        if (parent[u][j] != parent[v][j]) {
            res = max({res, max_jump[u][j], max_jump[v][j]});
            u = parent[u][j];
            v = parent[v][j];
        }
    }

    // Finally, include the values at the LCA and the nodes just below it
    res = max({res, max_jump[u][0], max_jump[v][0], a[parent[u][0] - 1]});
    return res;
}

vector<int> maximumPath(int n, vector<int> a, vector<vector<int>> edges, vector<vector<int>> queries) {
    vector<vector<int>> adj(n + 1);
    for (auto& e : edges) {
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]);
    }

    vector<int> depth(n + 1, 0);
    vector<vector<int>> parent(n + 1, vector<int>(MAX_LOG, 0));
    vector<vector<int>> max_jump(n + 1, vector<int>(MAX_LOG, 0));

    depth[0] = -1;
    dfs(1, 0, adj, depth, parent, max_jump, a);

    vector<int> ans;
    for (auto& q : queries) {
        ans.push_back(query_max(q[0], q[1], depth, parent, max_jump, a));
    }
    return ans;
}