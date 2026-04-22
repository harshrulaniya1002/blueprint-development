#include<bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        vector<vector<pair<int,int>>>adj(n);
        for(auto &edge: flights){
            adj[edge[0]].push_back({edge[1],edge[2]});
        }

        queue<pair<int,pair<int,int>>>q;
        vector<int>price(n,1e8);

        //stops, dist, node
        q.push({0,{0,src}});
        price[src] = 0;


        while(!q.empty()){
            int stops = q.front().first;
            int dist = q.front().second.first;
            int node = q.front().second.second;
            q.pop();
            if(stops > k) continue;

            for(auto it: adj[node]){
                int nxt = it.first;
                int fuel = it.second;
                if(dist+fuel < price[nxt]){
                    price[nxt] = fuel + dist;
                    q.push({stops+1,{price[nxt],nxt}});
                }
            }
        }

        if(price[dst] != 1e8) return price[dst];
        return -1;
    }
};