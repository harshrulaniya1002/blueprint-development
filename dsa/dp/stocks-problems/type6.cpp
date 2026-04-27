//Question: https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-transaction-fee/description/

// code:
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class Solution {
public:

    int n,Fee;
    vector<int>arr;
    int dp[50001][2];

    int func(int i, int buy){
        if(i>=n) return 0;
        if(dp[i][buy] != -1) return dp[i][buy];
        if(buy==1){
            return dp[i][buy] = max(-arr[i] + func(i+1,1-buy), func(i+1,buy));
        }

        return dp[i][buy] = max(arr[i]+func(i+1,1-buy)-Fee, func(i+1,buy));
    }
    int maxProfit(vector<int>& prices, int fee) {
        n=prices.size();
        arr=prices;
        Fee=fee;

        memset(dp,-1,sizeof(dp));
        return func(0,1);
    }
};