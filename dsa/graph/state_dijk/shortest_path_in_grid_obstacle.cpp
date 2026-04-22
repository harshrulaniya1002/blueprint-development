#include<bits/stdc++.h>
using namespace std;


class Solution {
public:
    int n,m;
    struct state {
        int i;
        int j;
        int obs;
        int dist;

        bool operator>(const state &other) const {
            return dist > other.dist;
        }
    };

    bool inrange(int x, int y){
        if(x<0 || y<0 || x>=n || y>=m) return 0;
        return 1;
    }
   
    int shortestPath(vector<vector<int>>& grid, int k) {
        n=grid.size();
        m=grid[0].size();

        priority_queue<state, vector<state>, greater<state>>pq;
        vector<vector<vector<int>>>dis(n,vector<vector<int>>(m,vector<int>(k+1,INT_MAX)));
        pq.push({0,0,0});
        dis[0][0][0]=0;

        vector<int>dirx = {-1,1,0,0};
        vector<int>diry = {0,0,1,-1};

        while(!pq.empty()){
            auto s = pq.top();
            pq.pop();
            int x = s.i;
            int y = s.j;
            int obs = s.obs;
            int d = s.dist; 

            if(x == n-1 && y == m-1) return d;
            if(obs > k || dis[x][y][obs] < d) continue;
            dis[x][y][obs] = d;

            for(int kk=0;kk<4;kk++){
                int nx = dirx[kk] + x;
                int ny = diry[kk] + y;

                if(inrange(nx,ny)){

                    int newobs = grid[nx][ny] + obs;
                    if((newobs <= k ) && dis[nx][ny][newobs] > d+1){
                        dis[nx][ny][newobs] = 1 + d;
                        pq.push({nx,ny,newobs,d+1});
                    }
                }
            }

        }

        return -1;
    }
};