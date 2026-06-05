#include<iostream>
using namespace std;


void dfs(int node,int parent,vector<vector<int>>& tree){
    cout<<node<<" ";
    for(int child:tree[node]){
        if(child!=parent){
            dfs(child,node,tree);
        }
    }
}

void bfs(int start,vector<vector<int>>& tree){
    vector<bool> visited(tree.size(),false);
    queue<int> q;
    q.push(start);
    visited[start]=true;

    while(!q.empty()){
        int node=q.front();
        q.pop();
        cout<<node<<" ";
        for(int child:tree[node]){
            if(!visited[child]){
                visited[child]=true;
                q.push(child);
            }
        }
    }
}

int main()
{
    int n;
    cin>>n;

    vector<vector<int>> tree(n);
    for(int i=0;i<n-1;i++){
        int u,v;
        cin>>u>>v;
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    //print tree
    for(int i=0;i<n;i++){
        cout<<i<<" : ";
        for(int j:tree[i]){
            cout<<j<<" ";
        }
        cout<<endl;
    }

    dfs(0,-1,tree);
    cout<<endl;
    bfs(0,tree);
}