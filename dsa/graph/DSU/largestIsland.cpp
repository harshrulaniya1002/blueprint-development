#include<iostream>
#include<set>
using namespace std;

class Solution {
public:
    class DSU{
        public:
        vector<int>parent;
        vector<int>rank;
        vector<int>size;



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



    int n;
    bool inrange(int x, int y){
        if(x<0 || y<0 || x>=n || y>=n) return 0;
        return 1;
    }
   
    int largestIsland(vector<vector<int>>& grid) {
        DSU ds;
        n=grid.size();
        ds.make_set(n*n);
  
        vector<int>dirx = {-1,1,0,0};
        vector<int>diry = {0,0,1,-1};

        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(grid[i][j]==1){
                    for(int k=0;k<4;k++){
                        int nx = dirx[k] + i;
                        int ny = diry[k] + j;

                        if(inrange(nx,ny) && grid[nx][ny] == 1){
                            int n1 = n*i + j;
                            int n2 = n*nx + ny;
                            if(ds.findParent(n1) != ds.findParent(n2)) ds.UnionBySize(n1,n2);
                        }
                    }
                }
            }
        }


        int ans=0;
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(grid[i][j]==0){
                    set<int>st;
                    for(int k=0;k<4;k++){
                        int nx = i+dirx[k];
                        int ny = j+diry[k];

                        if(inrange(nx,ny) && grid[nx][ny]==1){
                            int node = nx*n + ny;
                            st.insert(ds.findParent(node));
                        }
                    }

                    int res = 1;
                    for(auto it : st){
                        res += ds.size[it];
                    }

                    ans = max(ans,res);
                }
            }
        }

        for(int i=0;i<(n*n);i++){
            ans = max(ans, ds.size[ds.findParent(i)]);
        }
        return ans;

    }
};