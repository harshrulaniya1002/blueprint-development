#include<bits/stdc++.h>
using namespace std;

int n,m;
vector<vector<char>>adj;
vector<vector<int>>vis;    

vector<int>rx = {-1,1,0,0};
vector<int>ry = {0,0,-1,1};

bool inrange(int x, int y){
    if(x<0 || y<0 || x>=n || y>=m){
        return 0;
    }

    return 1;
}

void dfs(int x, int y){
    vis[x][y]=1;

    for(int i=0;i<4;i++){
        int nx = x + rx[i];
        int ny = y + ry[i];
        if(inrange(nx,ny) && !vis[nx][ny] && adj[nx][ny] == '.'){
            dfs(nx,ny);
        }
    }
}

int countingRooms(){
    int ans=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(!vis[i][j] && adj[i][j] == '.'){
                ans++;
                dfs(i,j);
            }
        }
    }
    return ans;
}

int main()
{
    cin>>n>>m;
    adj.resize(n,vector<char>(m));
    vis.resize(n,vector<int>(m,0));

    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            cin>>adj[i][j];
        }
    }

    cout<<countingRooms()<<"\n";
}