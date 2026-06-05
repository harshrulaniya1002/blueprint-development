#include<iostream>
#include<vector>

using namespace std;

vector<vector<int>>adj;
vector<int>subtree;

void dfs(int node, int par){
    subtree[node] = 0;
    for(auto child:adj[node]){
        if(child!=par){
            dfs(child,node);
            subtree[node] += 1+ subtree[child];
        }
    }
}

int main(){
    int n;
    cin>>n;
    adj.resize(n+1);
    subtree.resize(n+1,0);

    for(int i=2;i<=n;i++){
        int x;
        cin>>x;
        adj[x].push_back(i);
    }
    
    dfs(1,0);
    for(int i=1;i<=n;i++){
        cout<<subtree[i]<<" ";
    }
    cout<<endl;
}
