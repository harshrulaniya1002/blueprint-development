#include<bits/stdc++.h>
using namespace std;

vector<int>vis;
vector<vector<int>>adj;


//time complexity = O(N+M);
bool dfs(int node, int par){
    vis[node]=1;
    for(auto& it : adj[node]){
        if(!vis[it]){
            bool ans = dfs(it,node);
            if(ans) return true;
        }else if(it != par){
            return true;
        }
    }

    return false;
}

bool CycleDetected(int n, int m, vector<vector<int>>&adj){
    for(int i=0;i<n;i++){
        if(!vis[i]){
            if(dfs(i,-1)) return true;
        }
    }

    return false;
}

int main(){
    int n,m;
    cin>>n>>m;
    vis.resize(n,0);
    adj.resize(n);
  
    for(int i=0;i<m;i++){
        int x,y;
        cin>>x>>y;

        adj[x].push_back(y);
        adj[y].push_back(x);
    }

    cout<<"Cycle detected = "<<CycleDetected(n,m,adj)<<"\n";
}