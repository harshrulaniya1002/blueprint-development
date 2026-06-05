#include<bits/stdc++.h>
using namespace std;


const int depth_limit = 20;


void dfs(int node, int par, vector<int>&depth, vector<vector<int>>&parent, vector<vector<int>>&adj){
    depth[node] = depth[par]+1;
    parent[node][0] = par;
    
    for(int j=1; j< depth_limit; j++){
        parent[node][j] = parent[parent[node][j-1]][j-1];
    }

    for(auto it: adj[node]){
        if(it != par){
            dfs(it,node,depth,parent,adj);
        }
    }
}


//time complexity : O(NlogN) for preprocessing and O(logN) for each query
int LCA_method_2(int u, int v, vector<int>&depth, vector<vector<int>>&parent){
    if(u==v) return u;
    if(depth[u] < depth[v]) swap(u,v);

    int diff = depth[u]-depth[v];
    for(int j=depth_limit-1; j>=0; j--){
        if((1<<j) & diff){
            u = parent[u][j];
        }
    }
    
    if(u==v) return u;

    //now both are at same depth
    for(int j=depth_limit-1; j>=0; j--){
        if(parent[u][j] != parent[v][j]){
            u = parent[u][j];
            v = parent[v][j];
        }
    }
    return parent[u][0];
}



vector<int> LCA(int n, vector<vector<int>>edges, vector<vector<int>>queries)
{
    vector<vector<int>>parent(n+1,vector<int>(depth_limit,0));
    vector<int>depth(n+1,0);
    vector<vector<int>>adj(n+1);
    
    for(auto& it : edges){
        adj[it[0]].push_back(it[1]);
        adj[it[1]].push_back(it[0]);
    }
    
    depth[0]=-1;
    
    dfs(1,0,depth,parent,adj);
    vector<int>ans;
    for(auto it : queries){
        ans.push_back(LCA_method_2(it[0],it[1],depth,parent));
    }
    
    return ans;
    
}