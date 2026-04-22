#include<bits/stdc++.h>
using namespace std;

int n,m;

vector<vector<char>>mat;
vector<vector<int>>vis;    

vector<int>rx = {-1,1,0,0};
vector<int>ry = {0,0,-1,1};

bool inrange(int x, int y){
    if(x<0 || y<0 || x>=n || y>=m){
        return 0;
    }

    return 1;
}
 
void solve(){
    queue<pair<int,pair<int,int>>>q;
    vector<vector<pair<int,int>>>parent(n,vector<pair<int,int>>(m,{-1,-1}));
    vector<vector<int>>dist(n,vector<int>(m,INT_MAX));
    int ex,ey;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(mat[i][j] == 'A'){
                q.push({0,{i,j}});
                dist[i][j]=0;
            }else if(mat[i][j] == 'B'){
                ex=i;ey=j;
            }
        }
    }

    int shortestpath=0;

    while(!q.empty()){
        int dis = q.front().first;
        int x = q.front().second.first;
        int y = q.front().second.second;
        q.pop();

        if(dis > dist[x][y]) continue;
        if(x == ex && y == ey){
            shortestpath=dis;
        }

        for(int i=0;i<4;i++){
            int nx = x + rx[i];
            int ny = y + ry[i];

            if(inrange(nx,ny) && (mat[nx][ny] == '.' || mat[nx][ny] == 'B')  && (1 + dist[x][y] < dist[nx][ny])){
                dist[nx][ny] = 1 + dist[x][y];
                parent[nx][ny] = make_pair(x,y);
                q.push({1+dist[x][y],{nx,ny}});
            }
        }
    }

    if(dist[ex][ey] != INT_MAX){
        cout<<"YES"<<"\n"<<shortestpath<<"\n";
        string path = "";
        int x = ex, y = ey;

        while(parent[x][y] != make_pair(-1,-1)){
            int px = parent[x][y].first;
            int py = parent[x][y].second;

            if(px == x-1 && py == y) path += 'D';
            else if(px == x+1 && py == y) path += 'U';
            else if(px == x && py == y-1) path += 'R';
            else if(px == x && py == y+1) path += 'L';

            x = px;
            y = py;
        }

        reverse(path.begin(), path.end());
        cout<<path<<"\n";
    }else{
        cout<<"NO"<<"\n";
    }
}



int main()
{
    cin>>n>>m;
    mat.resize(n,vector<char>(m));
    vis.resize(n,vector<int>(m,0));

    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            cin>>mat[i][j];
        }
    }

    solve();
}