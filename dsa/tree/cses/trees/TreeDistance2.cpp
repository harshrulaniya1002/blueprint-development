#include<iostream>
#include<vector>
#define int long long
using namespace std;

vector<vector<int>>adj;
vector<int>subsum;
vector<int>dist;
vector<int>ans;

void dfs(int node, int par){
    subsum[node]=1;
    for(auto ch : adj[node]){
        if(ch!=par){
            dfs(ch,node);
            subsum[node] += subsum[ch];
            //subsum[ch] because edge from node->ch will be counted subsum[ch] times 
            // in the final answer as there are subsum[ch] nodes in the subtree of ch 
            // and each of those nodes will have a distance of 1 from node.
            dist[node] += dist[ch] + subsum[ch];
        }
    }
}

void rerootingDfs(int node, int par){
    ans[node] = dist[node];
    for(auto ch : adj[node]){
        if(ch != par){
            //removing contri from child node or the node which is being rerooted to
            int newdist = dist[node]-dist[ch]-subsum[ch];
            dist[ch] += newdist + (subsum[node]-subsum[ch]);
            subsum[ch] = subsum[node];
            rerootingDfs(ch,node);
        }   
    }
}

int32_t main(){
    int n;
    cin>>n;
    adj.resize(n+1);
    dist.resize(n+1,0);
    subsum.resize(n+1,0);
    ans.resize(n+1,0);

    for(int i=1;i<n;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs(1,-1);
    rerootingDfs(1,-1);

    for(int i=1;i<=n;i++){
        cout<<ans[i]<<" ";
    }

    cout<<endl;
}
