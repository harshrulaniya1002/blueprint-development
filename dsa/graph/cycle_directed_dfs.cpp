#include<bits/stdc++.h>
using namespace std;

vector<int>vis;
// vector<int>path
vector<vector<int>>adj;


//time complexity = O(N+M);
bool dfs(int node, int par){
    vis[node]=2;
    // path[node]=1;
    for(auto& it : adj[node]){
        if(vis[it]==0){
            bool ans = dfs(it,node);
            if(ans) return true;
        }else if(vis[it]==2){
            return true;
        }
    }

    vis[node]=1;

    return false;
}

bool CycleDetected(int n, int m, vector<vector<int>>&adj){
    for(int i=0;i<n;i++){
        if(!vis[i]){
            if(dfs(i,-1)) {
                cout<<"node = "<<i<<"\n";
                return true;
            }
        }
    }

    return false;
}

int main(){
    int n,m;
    cin>>n>>m;
    vis.resize(n,0);
    // path.resize(n,0);
    adj.resize(n);
  
    for(int i=0;i<m;i++){
        int x,y;
        cin>>x>>y;

        adj[x].push_back(y);
    }

    cout<<"Cycle Directed graph detected = "<<CycleDetected(n,m,adj)<<"\n";
}