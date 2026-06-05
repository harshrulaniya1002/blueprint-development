#include<iostream>
#include<vector>

using namespace std;

vector<vector<int>>adj;
vector<int>down;
vector<int>up;
vector<int>ans;

void dfs(int node, int par){
    for(auto ch : adj[node]){
        if(ch!=par){
            dfs(ch,node);
            down[node] = max(down[node],1+down[ch]);
        }
    }
}

void dfs2(int node, int par){
    int mx1=-1,mx2=-1;
    for(auto ch: adj[node]){
        if(ch != par){
            int val = down[ch]+1;
            if(val>mx1){
                mx2=mx1;
                mx1=val;
            }
            else if(val>mx2){
                mx2=val;
            }
        }
    }

    for(auto ch: adj[node]){
        if(ch != par){
            int use;
            if(1+down[ch] == mx1){
                use=mx2;
            }else{
                use=mx1;
            }

            up[ch] = 1 + max(up[node],max(0,use));
            dfs2(ch,node);
        }
    }
}

int main(){
    int n;
    cin>>n;
    adj.resize(n+1);
    down.resize(n+1,0);
    up.resize(n+1,0);
    ans.resize(n+1,0);

    for(int i=1;i<n;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    dfs(1,0);
    dfs2(1,0);
    for(int i=1;i<=n;i++){
        ans[i] = max(down[i], up[i]);
    }
    for(int i=1;i<=n;i++){
        cout<<ans[i]<<" ";
    }
    cout<<endl;
}
