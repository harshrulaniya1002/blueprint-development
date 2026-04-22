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
void dijkstra(){
    vector<int>dist(n,1e8);
    priority_queue<pair<int,int> , vector<pair<int,int>> , greater<pair<int,int>>>pq;
    dist[src]=0;
    pq.push({dist[src],src});

    while(!pq.empty()){
        int node = pq.top().second;
        int dis = pq.top().first;
        pq.pop();

        if(dist[node] < dis){
            continue;
        }

        for(auto it : adj[node]){
            int nxt = it.first;
            int wt = it.second;

            if(dist[node] + wt < dist[nxt]){
                dist[nxt] = wt + dist[node];
                pq.push({dist[nxt], nxt});
            }
        }
    }

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
    cin>>src;
    adj.resize(n);
    vis.resize(n,0);

    for(int i=0;i<m;i++){
        int x,y,z;
        cin>>x>>y>>z;
        adj[x].push_back({y,z});
    }

    dijkstra();
}