#include<bits/stdc++.h>
using namespace std;

int n,m;
vector<vector<pair<int,int>>>adj;
stack<int>st;
vector<int>vis;
int src;


//if graph is DAG , use this method for finding single source
//  source shorted path, time complexity O(V+E)

// alternate dijkstra, do same in O(ElogV);
void dfs(int node){
    vis[node]=1;
    for(auto it : adj[node]){
        if(!vis[it.first]){
            dfs(it.first);
        }
    }

    st.push(node);
}

void topo(){
    vector<int>dist(n,1e8);


    for(int i=0;i<n;i++){
        if(!vis[i]){
            dfs(i);
        }
    }

    dist[src]=0;

    while(!st.empty()){
        int node = st.top();
        st.pop();

        for(auto it : adj[node]){
            int nxt = it.first;
            int wt = it.second;

            if(dist[node] + wt < dist[nxt]){
                dist[nxt] = wt + dist[node];
            }
        }
    }

    for(int i=0;i<n;i++){
        if(dist[i] == 1e8){
            dist[i]= -1;
        }
    }

    cout<<"Distance from src to other nodes "<<"\n";
    for(int i=0;i<n;i++){
        cout<<dist[i]<<" ";
    }

    cout<<"\n";
}

int main()
{
    cin>>n>>m;
    cin>>src;
    adj.resize(n);
    vis.resize(n,0);

    for(int i=0;i<m;i++){
        int x,y,z;
        cin>>x>>y>>z;
        adj[x].push_back({y,z});
    }

    topo();
}