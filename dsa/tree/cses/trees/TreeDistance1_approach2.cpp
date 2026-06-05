#include<iostream>
#include<vector>

using namespace std;

vector<vector<int>>adj;
vector<int>depth;
vector<int>ans;

void dfs(int node, int par){

    for(auto ch : adj[node]){
        if(ch!=par){
            depth[ch] = depth[node]+1;
            dfs(ch,node);
        }
    }
}

int main(){
    int n;
    cin>>n;
    adj.resize(n+1);
    depth.resize(n+1,0);
    ans.resize(n+1,0);

    for(int i=1;i<n;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    if(n==1){
        cout<<0<<endl;
        return 0;
    }
    
    dfs(1,-1);
    int end1=-1;
    int dis=0;
    for(int i=1;i<=n;i++){
        if(depth[i] > dis){
            dis = depth[i];
            end1 = i;
        }
    }


    
    depth.assign(n+1,0);
    dfs(end1,-1);
    vector<int>distancefromEnd1 = depth;
    
    int end2=-1;
    dis=0;
    for(int i=1;i<=n;i++){
        if(depth[i] > dis){
            dis = depth[i];
            end2 = i;
        }
    }
    
    depth.assign(n+1,0);
    dfs(end2,-1);
    vector<int>distancefromEnd2 = depth;

    for(int i=1;i<=n;i++){
        ans[i] = max(distancefromEnd1[i], distancefromEnd2[i]);
    }

    for(int i=1;i<=n;i++){
        cout<<ans[i]<<" ";
    }
    cout<<endl;
}
