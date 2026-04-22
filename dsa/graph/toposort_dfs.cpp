#include<bits/stdc++.h>
using namespace std;

int n,m;
vector<vector<int>>adj;
vector<int>vis;
stack<int>st;

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

    st.push(node);
    return false;
}

void topoSort(){
    bool isCycle=false;
    for(int i=0;i<n;i++){
        if(!vis[i]){
            dfs(i);
            // isCycle = dfs(i);
            // if(isCycle){
            //     break;
            // }
        }
    }

    for(int i=0;i<n;i++){
        cout<<vis[i]<<" ";
    }
    cout<<"\n";
    

    // if(isCycle){
    //     cout<<"Cycle Detetected , Graph is not DAG"<<"\n";
    // }else{
    //     cout<<"Topo Ordering.."<<"\n";
    //     while(!st.empty()){
    //         cout<<st.top()<<" ";
    //         st.pop();
    //     }
    // }
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