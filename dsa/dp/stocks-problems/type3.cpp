//Question: https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iii/description/

// code:
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class Solution {
public:
    int n;
    vector<int>arr;
    int dp[100001][2][3];

    int func(int i, int buy, int cap){
        if(i>=n|| cap ==0) return 0;
        if(dp[i][buy][cap] != -1) return dp[i][buy][cap];
        if(buy==1){
            return dp[i][buy][cap]= max(-arr[i] + func(i+1,1-buy,cap), func(i+1,buy,cap));
        }

        return dp[i][buy][cap]= max(arr[i]+func(i+1,1-buy,cap-1), func(i+1,buy,cap));
    }
    int maxProfit(vector<int>& prices) {
        n=prices.size();
        arr=prices;

        memset(dp,-1,sizeof(dp));
        return func(0,1,2);
    }
};


