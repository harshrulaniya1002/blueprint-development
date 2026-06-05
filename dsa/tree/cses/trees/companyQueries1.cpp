#include<iostream>
#include<vector>
#include<cmath>
using namespace std;
const int MAXVAL = log2(2e5)+1;

vector<vector<int>>adj;
vector<vector<int>>parent;

void dfs(int node, int par){
    parent[node][0] = par;
    for(int i=1;i<MAXVAL;i++){
        if(parent[node][i-1] != -1){
            parent[node][i] = parent[parent[node][i-1]][i-1];
        }
    }

    for(auto ch : adj[node]){
        if(ch!=par){
            dfs(ch,node);
        }
    }
}

int query(int node, int k){
    for(int i=MAXVAL-1; i>=0;i--){
        if((1<<i) & k){
            node = parent[node][i];
            if(node == -1){
                return -1;
            }
        }
    }

    return node;
}


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,q;
    cin>>n>>q;
    adj.resize(n+1);
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

        //yth parent of x;
        cout<<query(x,y)<<"\n";
    }
}