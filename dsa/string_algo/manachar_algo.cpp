#include<iostream>
using namespace std;

class Solution {
public:

    // O(N^2)
    string method1(string& t){
        string s="#";
        for(char ch: t){
            s += ch;
            s += '#';
        }

        int n=s.size();
        int mlen=0;
        int stidx=0;
        for(int i=0;i<n;i++){
            int st=i;
            int end=i;
            int l=0;
            while((st >=0 && end < n) && (s[st] == s[end])){
                l++;
                st--;
                end++;
            }

            if(l>=mlen){
                mlen=l;
                stidx=i;
            }
        }
        

        int st = (stidx-mlen+1)/2;
        return t.substr(st,mlen-1);
    }

    //O(N)
    string manachar_algo(string& s){
        string t="#";
        for(char ch: s){
            t += ch;
            t += '#';
        }

        int n=t.size();
        vector<int>p(n,0);


        int c=0,r=0;
        int lc=0,lr=0;

        for(int i=0;i<n;i++){
            int mirror = 2*c - i;

            if(i<r){
                p[i]= min(p[mirror], r-i);
            }

            while(((i-p[i]-1 >= 0) && (i+p[i]+1 < n)) && (t[i-p[i]-1] == t[i+p[i]+1])){
                p[i]++;
            }

            if(i+p[i] > r){
                r = i+p[i];
                c = i;
            }


            if(p[i] > lr){
                lr = p[i];
                lc = i;
            }
        }

        int stidx = (lc-lr)/2;
        return s.substr(stidx,lr);
    }
    string longestPalindrome(string s) {
        return method1(s);
        // return lPallindromString(s);
    }
};