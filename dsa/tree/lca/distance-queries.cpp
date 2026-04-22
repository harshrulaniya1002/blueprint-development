#include<bits/stdc++.h>
using namespace std;

const int max_limit = log2(1e5)+1;

void dfs(int node, int par, vector<vector<int>>&adj,  vector<int>&depth, vector<vector<int>>&parent){
    depth[node] = depth[par] + 1;
    parent[node][0] = par;
    
    for(int j=1;j<max_limit;j++){
        parent[node][j] = parent[parent[node][j-1]][j-1];
    }
    
    for(auto &it : adj[node]){
        if(it != par){
            dfs(it,node,adj,depth,parent);
        }
    }
}

int LCA(int u, int v, vector<int>&depth, vector<vector<int>>&parent){
    if(u==v) return u;
    if(depth[u] < depth[v]) swap(u,v);
    
    
    int diff = depth[u]-depth[v];
    
    for(int j=max_limit-1; j>=0; j--){
        if((1<<j) & diff){
            u = parent[u][j];
        }
    }
    
    if(u==v) return u;
    
    
    for(int j=max_limit-1; j>=0 ; j--){
        if(parent[u][j] != parent[v][j]){
            u = parent[u][j];
            v = parent[v][j];
        }
    }
    
    return parent[u][0];
}


vector<int> distanceQuery(int n, vector<vector<int>>edges, vector<vector<int>>queries)
{
    vector<vector<int>>adj(n+1);
    vector<int>depth(n+1,0);
    vector<vector<int>>parent(n+1,vector<int>(max_limit,0));
    
    for(auto& it : edges){
        adj[it[0]].push_back(it[1]);
        adj[it[1]].push_back(it[0]);
    }
    
    depth[0]=-1;
    
    
    dfs(1,0,adj,depth,parent);
    
    vector<int>ans;
    for(auto it : queries){
        int lca = LCA(it[0], it[1], depth, parent);
        ans.push_back(depth[it[0]] + depth[it[1]] - (2*depth[lca]));
    }
    return ans;
}