#include<iostream>
#include<vector>
#include<cmath>
using namespace std;
const int MAXVAL = log2(2e5)+1;

vector<vector<int>>adj;
vector<vector<int>>parent;
vector<int>depth;

void dfs(int node, int par){
    parent[node][0] = par;
    for(int i=1;i<MAXVAL;i++){
        if(parent[node][i-1] != -1){
            parent[node][i] = parent[parent[node][i-1]][i-1];
        }
    }

    for(auto ch : adj[node]){
        if(ch!=par){
            depth[ch] = depth[node] + 1;
            dfs(ch,node);
        }
    }
}

int query(int u, int v){
    if(u==v) return u;
    if(depth[u] < depth[v]) swap(u,v);


    int diff = depth[u]-depth[v];
    for(int i=MAXVAL-1; i>=0;i--){
        if((1<<i) & diff){
            u = parent[u][i];
        }
    }

    if(u==v) return u;
    for(int i=MAXVAL-1; i>=0;i--){
        if(parent[u][i] != parent[v][i]){
            u = parent[u][i];
            v = parent[v][i];
        }
    }

    return parent[u][0];
}


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,q;
    cin>>n>>q;
    adj.resize(n+1);
    depth.resize(n+1,0);
    parent.resize(n+1,vector<int>(MAXVAL,-1));
    for(int i=2;i<=n;i++){
        int x;
        cin>>x;
        adj[x].push_back(i);
        adj[i].push_back(x);
    }

    dfs(1,-1);


    while(q--){
        int x,y;
        cin>>x>>y;

        //lca(x,y)
        cout<<query(x,y)<<"\n";
    }
}