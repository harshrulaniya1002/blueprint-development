#include<bits/stdc++.h>
using namespace std;

class Solution {
public:
    /**
     * @param n: number of city
     * @param roads: Specific information on each road
     * @param discounts: Number of discounts
     * @return: Minimum cost from city 0 to city n - 1
     */
    int minimumCost(int n, vector<vector<int>> &roads, int discounts) {
        // write your code here
        vector<vector<pair<int,int>>>adj(n);
        for(auto it : roads){
            adj[it[0]].push_back({it[1],it[2]});
            adj[it[1]].push_back({it[0],it[2]});
        }

        priority_queue<pair<int,pair<int,int>>, vector<pair<int,pair<int,int>>>, greater<pair<int,pair<int,int>>>>q;

        //state ({dist,isCouponUsed})
        vector<vector<int>>dist(n,vector<int>(discounts+1,INT_MAX));

        //{dist,{node,noOfDiscountsusedTillNow}}
        q.push({0,{0,0}});
        dist[0][0]=0;

        while(!q.empty()){
            int cost = q.top().first;
            int discount = q.top().second.second;
            int node = q.top().second.first;
            q.pop();

            if(cost > dist[node][discount]) continue;
            if(node == n-1) return cost;

            for(auto it : adj[node]){
                int nxt = it.first;
                int wt = it.second;

                //no discount
                if(dist[nxt][discount] > cost + wt){
                    dist[nxt][discount] = cost + wt;
                    q.push({cost+wt,{nxt,discount}});
                }


                //with discount
                if(discount < discounts && dist[nxt][discount+1] > cost + wt/2){
                    dist[nxt][discount+1] = cost + wt/2;
                    q.push({cost+wt/2,{nxt,discount+1}});
                }
            }
        }

        return -1;
        
    }
};