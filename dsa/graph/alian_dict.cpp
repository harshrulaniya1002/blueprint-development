#include<bits/stdc++.h>
using namespace std;

class Solution {
  public:
    vector<vector<int>>adj;
    vector<int>vis;
    stack<int>st;
    vector<int>present;
    int k;
    
    bool dfs(int node){
        vis[node]=1;
        for(auto it: adj[node]){
            if(!vis[it]){
                int ans = dfs(it);
                if(ans) return true;
            }else if(vis[it]==1){
                return true;
            }
        }
        vis[node]=2;
        st.push(node);
        return false;
    }
    
    string topoSort(){
        
        for(int i=0;i<k;i++){
            if(!vis[i] && present[i]){
                bool res = dfs(i);
                if(res){
                    return "";
                }
            }
        }
        
        string ans="";
        while(!st.empty()){
            int node=st.top();
            
            if(present[node]) ans.push_back(char(st.top()+'a'));
            st.pop();
        }
        return ans;
    }
    
    
    string findOrder(vector<string> &words) {
        
        // code here
        int n=words.size();
        k=26;
        adj.resize(k);
        vis.resize(k,0);
        present.resize(26,0);
        
        for(auto &w : words){
            for(char c: w){
                present[c-'a']=1;
            }
        }
        
        
        for(int i=0;i<n-1;i++){
            string s1 = words[i];
            string s2 = words[i+1];
            bool chk=false;
            int len = min(s1.size(), s2.size());
            for(int j=0;j<len;j++){
                if(s1[j] != s2[j]){
                    adj[s1[j]-'a'].push_back(s2[j]-'a');
                    chk=true;
                    break;
                }
            }
            
            if(!chk && s1.size() > s2.size()) return "";
        }
        return topoSort();
    }
};