#include<iostream>
using namespace std;

int n,m;
vector<vector<pair<int,int>>>adj;

struct State{
    int wt;
    int node;
    int parent; // if we want to print the MST

    bool operator>(const State& others) const {
        return wt > others.wt;
    }
};


//time complexity -> O(ElogE)

void Prims(){
    vector<int>vis(n,0);
    priority_queue<State, vector<State>, greater<State>>pq;
    pq.push({0,0,-1});


    int weight=0;
    vector<vector<int>>mst;
    while(!pq.empty()){
        auto s = pq.top();
        pq.pop();

        int wt = s.wt;
        int node = s.node;
        int parent = s.parent;

        if(vis[node]) continue;
        vis[node]=1;
        weight += wt;
        if(parent != -1) mst.push_back({node,parent,wt});

        for(auto edge: adj[node]){
            int nxt = edge.first;
            int len = edge.second;
            if(!vis[nxt]){
                pq.push({len,nxt,node});
            }
        }
    }

    cout<<"Total Cost of MST "<<weight<<"\n";
    cout<<"Edges are: "<<"\n";
    for(auto it: mst){
        cout<<it[0]<<" "<<it[1]<<" "<<it[2]<<"\n";
    }
}

int main()
{
    cin>>n>>m;
    adj.resize(n);
    for(int i=0;i<m;i++){
        int x,y,z;
        cin>>x>>y>>z;
        adj[x].push_back({y,z});
        adj[y].push_back({x,z});
    }

    Prims();
}