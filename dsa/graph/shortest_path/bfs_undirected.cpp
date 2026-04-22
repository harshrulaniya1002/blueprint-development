#include<bits/stdc++.h>
using namespace std;

int n,m;
vector<vector<int>>adj;


void bfs(){
    queue<int>q;
    vector<int>dist(n,1e8);

    q.push(0);
    dist[0]=0;
    while(!q.empty()){
        int node = q.front();
        q.pop();

        for(auto it : adj[node]){
            if(dist[node]+1 < dist[it]){
                dist[it] = 1+ dist[node];
                q.push(it);
            }
        }
    }

    cout<<"distance from "<<0<<" to other nodes"<<"\n";
    for(int i=0;i<n;i++){
        if(dist[i] == 1e8){
            cout<<-1<<" ";
        }else{
            cout<<dist[i]<<" ";
        }
    }

    cout<<"\n";
}

int main()
{
    cin>>n>>m;
    adj.resize(n);

    for(int i=0;i<m;i++){
        int x,y;
        cin>>x>>y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }

    bfs();
}