#include<bits/stdc++.h>
using namespace std;

// Q: https://leetcode.com/problems/number-of-pairs-after-increment/

class Solution {
public:
    int n;
    int blockSize;
    vector<long long>nums;
    vector<unordered_map<long long,long long>>freq;
    vector<long long>lazy;

    void push(int blk){
        if(lazy[blk] == 0) return ;
        int st = blk*blockSize;
        int end = min(n-1,st+blockSize-1);

        for(int i=st; i<=end ;i++){
            nums[i] += lazy[blk];
        } 

        lazy[blk]=0;
    }

    void rebuild(int b){
        freq[b].clear();
        int st = b*blockSize;
        int end = min(n-1,st+blockSize-1);

        for(int i=st;i<=end;i++){
            freq[b][nums[i]]++;
        }
    }

    void rangeUpdate(int l, int r, int val){

        int lblk = l/blockSize;
        int rblk = r/blockSize;

        if(lblk == rblk){
            push(lblk);
            for(int i=l; i<=r;i++){
                nums[i] += val;
            }

            rebuild(lblk);
        }else{
            push(lblk);
            int leftEnd = (lblk+1)*blockSize-1;
            for(int i=l;i<=leftEnd;i++){
                nums[i] += val;
            }

            rebuild(lblk);


            for(int b=lblk+1;b<rblk;b++){
                lazy[b] += val;
            }

            push(rblk);
            int rightStart = rblk*blockSize;
            for(int i=rightStart;i<=r;i++){
                nums[i] += val;
            }

            rebuild(rblk);
        }
    }

    int query(int x){
        int ans=0;
        for(int b=0;b<blockSize;b++){
            int need = x-lazy[b];
            if(freq[b].count(need)){
                ans += freq[b][need];
            }
        }

        return ans;
    }
    
    vector<int> numberOfPairs(vector<int>& nums1, vector<int>& nums2, vector<vector<int>>& queries) {
       
       n=nums2.size();
       nums.resize(n);
       blockSize = ceil(sqrt(n));
       lazy.resize(blockSize,0);
       freq.resize(blockSize);
        
        //build
        for(int i=0;i<blockSize;i++){
            freq[i].clear();
        }
        for(int i=0;i<n;i++){
            int blockIdx = i/blockSize;
            nums[i]=nums2[i];
            freq[blockIdx][nums[i]]++;
        }

       vector<int>ans;
       for(auto it : queries){
           if(it[0]==1){
               int l = it[1];
               int r = it[2];
               int val= it[3];
               rangeUpdate(l,r,val);
           }else{
                int res=0;
                for(int i=0;i<nums1.size();i++){
                int find = it[1]-nums1[i];
                res += query(find);
               }
               ans.push_back(res);
           }
       }
       return ans;
    }
};



