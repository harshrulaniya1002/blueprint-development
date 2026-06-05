#include<iostream>
#include<vector>
#include<cmath>
#include<cstdint>

using namespace std;
#define int long long
vector<vector<int>>adj;
vector<int>val;
vector<int>inTime;
vector<int>outTime;
vector<int>euler;
int timer=0;

struct SegmentTree{
    vector<int> segtree;
    int n;

    public:
    SegmentTree(vector<int>&arr){
        n = arr.size();
        segtree.resize(4*n,0);
        build(0,0,n-1,arr);
    }

    // tc -> O(N) for building the segment tree
    // sc -> O(N) for storing the segment tree
    void build(int node, int l, int r, vector<int> &arr)
    {   
        if(l == r)
        {
            segtree[node] = arr[l];
            return;
        }
        int mid = (l+r)/2;
        build(2*node+1,l,mid,arr);
        build(2*node+2,mid+1,r,arr);
        segtree[node] = segtree[2*node+1] + segtree[2*node+2];
    }

    // Point Update Query in Segment Tree
    // tc -> O(logN) for updating a value in the segment tree
    // sc -> O(1) no extra space is required
    void updatePoint(int idx, int val, int node, int l, int r)
    {
        if(l == r){
            segtree[node] = val;
            return;
        }
        int mid = (l+r)/2;
        if(idx <= mid){
            updatePoint(idx,val,2*node+1,l,mid);
        }
        else{
            updatePoint(idx,val,2*node+2,mid+1,r);
        }
        segtree[node] = segtree[2*node+1] + segtree[2*node+2];
    }

    // TC -> O(LogN) for range sum query
    // SC -> O(1) no extra space is required
    int rangeSum(int node, int start, int end, int ql, int qr)
    {
        if(ql > end || qr < start) return 0; // no overlap
        if(ql <= start && qr >= end) return segtree[node]; // complete overlap
        else{
            int mid = (start+end)/2;
            int left = rangeSum(2*node+1,start,mid,ql,qr);
            int right = rangeSum(2*node+2,mid+1,end,ql,qr);
            return left + right;
        }
    }
};

void dfs(int node, int par){
    inTime[node] = timer;
    euler.push_back(val[node]);
    timer++;
    for(auto ch : adj[node]){
        if(ch!=par){
            dfs(ch,node);
        }
    }
    outTime[node] = timer-1;
}



int32_t main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,q;
    cin>>n>>q;
    adj.resize(n+1);
    val.resize(n+1);
    inTime.resize(n+1,0);
    outTime.resize(n+1,0);
    for(int i=1;i<=n;i++){
        cin>>val[i];
    }

    for(int i=0;i<n-1;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    dfs(1,-1);

    SegmentTree t(euler);

    while(q--){
        int type;
        cin>>type;
        if(type==1){
            int node,y;
            cin>>node>>y;
            val[node] = y;
            t.updatePoint(inTime[node],y,0,0,euler.size()-1);
        }else{
            int node;
            cin>>node;

            int leftIdx = inTime[node];
            int rightIdx = outTime[node];
            // all nodes whose start time greater than node and less than the endtime of node
            cout<<t.rangeSum(0,0,euler.size()-1,leftIdx,rightIdx)<<endl;
        }
    }
}
