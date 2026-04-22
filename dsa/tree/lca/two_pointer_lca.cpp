#include<iostream>
#include<queue>
#include<vector>
using namespace std;

vector<int>depth,parent;
vector<vector<int>>adj;

void dfs(int node, int par){
    depth[node] = depth[par]+1;
    parent[node] = par;
    for(auto it: adj[node]){
        if(it != par){
            dfs(it,node);
        }
    }
}


//time complexity : O(N) for preprocessing and O(N) for each query
int LCA_method_1(int u , int v){
    if(u==v) return u;
    if(depth[u] < depth[v]) swap(u,v);
    
    int diff = depth[u]-depth[v];
    while(diff--){
        u = parent[u];
    }
    
    while(u != v){
        u = parent[u];
        v = parent[v];
    }
    
    return u;
}



int main()
{
    int n,m;
    cin>>n>>m;

    adj.resize(n+1);
    depth.resize(n+1);
    parent.resize(n+1);
    for(int i=0;i<m;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs(1,0);
    cout<<LCA_method_1(2,5)<<"\n";
}