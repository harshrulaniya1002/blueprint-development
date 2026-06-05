
// Q: https://leetcode.com/problems/min-cost-climbing-stairs/

#include <vector>
#include <iostream>
using namespace std;

class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int n= cost.size();
        cost.resize(n+1);
        cost[n]=0;
        vector<int>dp(n+1,0);
        dp[0]=cost[0];
        dp[1]=cost[1];
        for(int i=2;i<=n;i++){
            dp[i] = min(dp[i-1], dp[i-2])+cost[i];
            cout<<i<<" "<<dp[i]<<"\n";
        }

        return dp[n];

    }
};