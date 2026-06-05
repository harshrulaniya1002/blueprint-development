#include<iostream>
#include<vector>

using namespace std;

vector<vector<int>>adj;
vector<int>vis;
int ans=0;
void dfs(int node, int par){
    for(auto child:adj[node]){
        if(child!=par){
            dfs(child,node);
            if(!vis[node] && !vis[child]){
                ans++;
                vis[node] = 1;
                vis[child] = 1;
            }
        }
    }
}

int main(){
    int n;
    cin>>n;
    adj.resize(n+1);
    vis.resize(n+1,0);

    for(int i=1;i<n;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    dfs(1,0);
    cout<<ans<<endl;
}
