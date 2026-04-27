//Question: https://leetcode.com/problems/best-time-to-buy-and-sell-stock/description/


// code:
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n= prices.size();

        int maxiProfit=0;
        int mini=INT_MAX;
        for(int i=0;i<n;i++){
            mini = min(mini,prices[i]); //minimum prices seen till now
            maxiProfit = max(maxiProfit, prices[i]-mini); // want to sell today
        }

        return maxiProfit;
    }
};
