// Q: https://leetcode.com/problems/climbing-stairs/

class Solution {
public:
    int climbStairs(int n) {
        int prev,prevl;
        if(n==1) return 1;
        prev=2;
        prevl=1;
        for(int i=2;i<n;i++){
            int curr = prev+prevl;
            prevl=prev;
            prev=curr;
        }

        return prev;
    }
};