#include<bits/stdc++.h>
using namespace std;

int n,m;
vector<vector<pair<int,int>>>adj;

class DSU{
    vector<int>parent;
    vector<int>rank;
    vector<int>size;

    public:

    void make_set(int n){
        parent.resize(n+1);
        rank.resize(n+1,0);
        size.resize(n+1,1);

        for(int i=0;i<=n;i++){
            parent[i]=i;
        }
    }

    int findParent(int x){
        if(x==parent[x]){
            return x;
        }

        return parent[x] = findParent(parent[x]);
    }

    void UnionByRank(int x, int y){
        int pu = findParent(x);
        int pv = findParent(y);
        if(pu == pv){
            return ;
        }

        if(rank[pu] < rank[pv]){
            parent[pu] = pv;
        }else if(rank[pv] < rank[pu]){
            parent[pv] = pu;
        }else{
            parent[pv] = pu;
            rank[pu]++;
        }
    }

    void UnionBySize(int x, int y){
        int pu = findParent(x);
        int pv = findParent(y);
        if(pu == pv){
            return ;
        }

        if(size[pu] < size[pv]){
            parent[pu] = pv;
            size[pv] += size[pu];
        }else{
            parent[pv] = pu;
            size[pu] += size[pv];
        }
    }
};

// tc-> O(ElogE + E*(α(N))) ~ O(ElogE)
void Kruskals(DSU& ds){
    ds.make_set(n+1);
    vector<pair<int,pair<int,int>>> edges;
    for(int i=1;i<=n;i++){
        for(auto edge : adj[i]){
            int n1 = i;
            int n2 = edge.first;
            int wt= edge.second;

            edges.push_back({wt,{n1,n2}});
        }
    }

    sort(edges.begin(),edges.end());
    int weight=0;
    for(auto edge : edges){
        int n1 = edge.second.first;
        int n2 = edge.second.second;
        if(ds.findParent(n1) != ds.findParent(n2)){
            weight+= edge.first;
            ds.UnionBySize(n1,n2);
        }
    }

    cout<<"cost of MST is "<<weight<<"\n";
}


int main(){
    cin>>n>>m;
    adj.resize(n+1);
    for(int i=0;i<m;i++){
        int x,y,wt;
        cin>>x>>y>>wt;
        adj[x].push_back({y,wt});
        adj[y].push_back({x,wt});
    }
    DSU ds;
    Kruskals(ds);
}