#include<iostream>
#include<vector>

using namespace std;

vector<vector<int>>adj;

int ans=0;

int dfs(int node, int par){
    int maxi=0;
    int dia1=0;
    int dia2=0;
    for(auto child:adj[node]){
        if(child!=par){
            int val = dfs(child,node);
            maxi = max(maxi,val);
            if(val>dia1){
                dia2 = dia1;
                dia1 = val;
            }
            else if(val>dia2){
                dia2 = val;
            }

        }
    }
    ans = max(ans,dia1+dia2);
    return maxi+1;
}

int main(){
    int n;
    cin>>n;
    adj.resize(n+1);

    for(int i=1;i<n;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    dfs(1,0);
    cout<<ans<<endl;
}
