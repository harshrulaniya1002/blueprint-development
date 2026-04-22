#include<bits/stdc++.h>
using namespace std;

int n,m,src;
vector<vector<int>>adj;

//time complexity O(V*E) , E is no of edges
void BellmanFord(){
    vector<int>dist(n,INT_MAX);
    dist[src]=0;
    queue<int>q;
    q.push(0);

    for(int i=0;i<n-1;i++){
        
        for(auto edges : adj){
            int n1 = edges[0];
            int n2 = edges[1];
            int wt = edges[2];

            if(dist[n1] != INT_MAX && (dist[n1] + wt < dist[n2])){
                dist[n2] = wt + dist[n1];
            }
        }
    }


    bool isnegcycle=false;
    for(auto edges : adj){
        int n1 = edges[0];
        int n2 = edges[1];
        int wt = edges[2];

        if(dist[n1] != INT_MAX && dist[n1] + wt < dist[n2]){
            isnegcycle=true;
            dist[n2] = wt + dist[n1];
        }
   }

   if(isnegcycle){
    cout<<"NegCycle exist"<<"\n";
   }else{
        for(int i=0;i<n;i++){
            if(dist[i] == INT_MAX) {
                cout<<-1<<" ";
            }else{
                cout<<dist[i]<<" ";
            }
        }

        cout<<"\n";
   }
}

int main()
{
    cin>>n>>m>>src;
    for(int i=0;i<m;i++){
        int x,y,z;
        cin>>x>>y>>z;
        adj.push_back({x,y,z});
    }

    BellmanFord();
}