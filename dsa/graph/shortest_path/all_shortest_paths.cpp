#include<bits/stdc++.h>
using namespace std;

class Solution {
public:
    const int mod =1e9+7;
    int countPaths(int n, vector<vector<int>>& roads) {
        vector<vector<pair<int,int>>>adj(n);
        for(auto it : roads){
            adj[it[0]].push_back({it[1],it[2]});
            adj[it[1]].push_back({it[0],it[2]});
        }


        vector<long long>dist(n,LLONG_MAX);
        vector<int>ways(n,0);
        priority_queue<pair<long long,int> , vector<pair<long long,int>>, greater<pair<long long,int>>>pq;
        pq.push({0,0});
        dist[0]=0;
        ways[0]=1;

        while(!pq.empty()){
            int node = pq.top().second;
            int time = pq.top().first;
            pq.pop();

            if(dist[node] < time) continue;

            for(auto it: adj[node]){
                int nxt= it.first;
                int wt = it.second;
                if(dist[node] + wt < dist[nxt]){
                    dist[nxt] = wt + dist[node];
                    ways[nxt] = ways[node];
                    pq.push({dist[nxt], nxt});
                }else if(dist[node] + wt == dist[nxt]){
                     ways[nxt] = (ways[nxt] + ways[node])%mod;
                }
            }
        }

        return ways[n-1];
    }
};