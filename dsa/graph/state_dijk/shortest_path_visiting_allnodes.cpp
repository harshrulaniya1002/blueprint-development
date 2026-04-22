// You have an undirected, connected graph of n nodes labeled from 0 to n - 1. 
// You are given an array graph where graph[i] is a list of all the nodes connected with node i by an edge.
// Return the length of the shortest path that visits every node. 
// You may start and stop at any node, you may revisit nodes multiple times, and you may reuse edges.

#include<bits/stdc++.h>
using namespace std;

class Solution {
public:

    struct State{
        int node;
        int dist;
        int mask;

        bool operator>(const State &other) const {
            return dist > other.dist;
        }
    };


    int shortestPathLength(vector<vector<int>>& graph) {
        int n=graph.size();
        priority_queue<State, vector<State> , greater<State>>pq;
        vector<vector<int>>dis(n,vector<int>(1<<n,INT_MAX));
        for(int i=0;i<n;i++) {
            pq.push({i,0,1<<i});
            dis[i][1<<i]=0;
        }


        while(!pq.empty()){
            auto s = pq.top();
            pq.pop();

            int node = s.node;
            int dist = s.dist;
            int mask = s.mask;
       
            if(mask == (1<<n)-1) return dist;
            if(dist > dis[node][mask]) continue;

            for(auto it : graph[node]){
                int newmask = mask | (1<<it);
                if(dis[it][newmask] > dis[node][mask] + 1){
                    dis[it][newmask] = dis[node][mask] + 1;
                    pq.push({it,dis[it][newmask], newmask});
                }
            }
        }

        return -1;
    }
};