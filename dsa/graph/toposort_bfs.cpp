#include<bits/stdc++.h>
using namespace std;

int n,m;
vector<vector<int>>adj;
vector<int>vis;


bool dfs(int node){
    vis[node]=2;
    for(auto &it : adj[node]){
        if(!vis[it]){
            bool cycle = dfs(it);
            if(cycle==true) return true;
        }else if (vis[it] == 2){
            return true;
        }
    }

    vis[node]=1;
    return false;
}

void topoSort(){
    bool isCycle=false;
    for(int i=0;i<n;i++){
        if(!vis[i]){
            isCycle = dfs(i);
            if(isCycle){
                break;
            }
        }
    }
    

    if(isCycle){
        cout<<"Cycle Detected , Graph is not DAG Using DFS"<<"\n";
    }

    vector<int>topo;
    queue<int>q;
    vector<int>indeg(n,0);
    for(int i=0;i<n;i++){
        for(auto& neigh: adj[i]){
            indeg[neigh]++;
        }
    }

    for(int i=0;i<n;i++){
        if(indeg[i]==0) q.push(i);
    }


    while(!q.empty()){
        int node = q.front();
        q.pop();
        topo.push_back(node);

        for(auto it : adj[node]){
            indeg[it]--;
            if(indeg[it]==0){
                q.push(it);
            }
        }
    }

 

    if(topo.size() != n){
        for(auto it: topo){
            cout<<it<<" ";
        }
        cout<<"\n";

        for(int i=0;i<n;i++){
            cout<<vis[i]<<" ";
        }
        cout<<"\n";
        cout<<"Cycle Detected , Graph is not DAG Using Kahn's BFS"<<"\n";
    }else{
        cout<<"topo Ordering using BFS.."<<"\n";
        for(auto it : topo){
            cout<<it<<" ";
        }
        cout<<"\n";
    }
}

int main()
{
    cin>>n>>m;
    adj.resize(n);
    vis.resize(n,0);

    for(int i=0;i<m;i++){
        int x,y;
        cin>>x>>y;
        adj[x].push_back(y);
    }

    topoSort();
}