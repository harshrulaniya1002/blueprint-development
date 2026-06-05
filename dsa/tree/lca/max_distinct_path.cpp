#include<bits/stdc++.h>
using namespace std;
// 2846. Minimum Edge Weight Equilibrium Queries in a Tree

class Solution {
public:
    const int D = 25;
    vector<vector<pair<int,int>>>adj;
    vector<vector<int>>parent;
    vector<vector<int>>Q;
    vector<int>depth;
    unordered_set<int>weights;

    void dfs(int node, int par){
        depth[node] = depth[par] + 1;
        parent[node][0] = par;

        for(int j=1;j<D;j++){
            parent[node][j] = parent[parent[node][j-1]][j-1];
        }

        for(auto &it : adj[node]){
            if(it.first != par){
                for(auto w : weights){
                    if(it.second == w){
                        Q[w][it.first] = Q[w][node] + 1;
                    }else{
                        Q[w][it.first] = Q[w][node];
                    }
                }

                dfs(it.first, node);
            }
        }
    }

    int lca(int u , int v){
        if(u==v) return u;
        if(depth[u] < depth[v]) swap(u,v);

        int diff = depth[u] - depth[v];
        for(int j = D-1;j>= 0 ;j--){
            if((1<<j) & diff){
                u = parent[u][j];
            }
        }

        if(u == v) return u;

        for(int j=D-1; j>=0 ;j--){
            if(parent[u][j] != parent[v][j]){
                u = parent[u][j];
                v = parent[v][j];
            }
        }

        return parent[u][0];
    }


    vector<int> minOperationsQueries(int n, vector<vector<int>>& edges, vector<vector<int>>& queries) {

        adj.resize(n+1);
        parent.resize(n+1, vector<int>(D,0));
        Q.resize(27, vector<int>(n+1,0));
        depth.resize(n+1,0);

        for(auto &it : edges){
            adj[it[0]].push_back({it[1],it[2]});
            adj[it[1]].push_back({it[0],it[2]});
            weights.insert(it[2]);
        }

        dfs(0,0);

        vector<int>ans(queries.size());
        for(int i=0;i<queries.size();i++){
            int u = queries[i][0];
            int v = queries[i][1];
            int LCA = lca(u,v);
            int path_len = depth[u] + depth[v] - 2*depth[LCA];

            int res = path_len;
            for(auto wt : weights){
                res = min(res, path_len - (Q[wt][u] + Q[wt][v] - 2*(Q[wt][LCA])));
            }

            ans[i] = res;
        }
        return ans;
    }
};