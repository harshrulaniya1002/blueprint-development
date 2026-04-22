#include<bits/stdc++.h>
using namespace std;

struct Node {
    char l,r;
    int count;
};

vector<Node> segtree;

void merge(int leftChild, int rightChild, int parent)
{
    Node &left = segtree[leftChild];
    Node &right = segtree[rightChild];

    int lr = left.r;
    int rl = right.l;

    if(lr == rl)
    {
        segtree[parent].count = left.count + right.count - 1;
    }
    else
    {
        segtree[parent].count = left.count + right.count;
    }
    segtree[parent].l = left.l;
    segtree[parent].r = right.r;
}

void build(int node, int l, int r, vector<int> &arr)
{   
    if(l == r)
    {
        segtree[node].l = segtree[node].r = arr[l];
        segtree[node].count = 1;
        return;
    }
    int mid = (l+r)/2;
    build(2*node+1,l,mid,arr);
    build(2*node+2,mid+1,r,arr);
    merge(2*node+1,2*node+2, node);
}


void update(int node, int start, int end, int idx)
{
    if(start == end)
    {
        segtree[node].l = segtree[node].r = (1- segtree[node].l);
        segtree[node].count = 1;
        return;
    }
    int mid = (start+end)/2;
    if(idx <= mid)
    {
        update(2*node+1,start,mid,idx);
    }
    else
    {
        update(2*node+2,mid+1,end,idx);
    }
    merge(2*node+1,2*node+2,node);
}


int query(int node, int start, int end, int l, int r)
{
    if(l > end || r < start) return 0; // no overlap
    if(l <= start && r >= end) return segtree[node].count; // complete overlap
    else{
        int mid = (start+end)/2;
        int left = query(2*node+1,start,mid,l,r);
        int right = query(2*node+2,mid+1,end,l,r);
        
        // combine left and right results
        if(left == 0) return right;
        if(right == 0) return left;

        int lr = segtree[2*node+1].r;
        int rl = segtree[2*node+2].l;

        if(lr == rl)
        {
            return left + right - 1;
        }
        else
        {
            return left + right;
        }
    }
}


int main()
{
    string s;
    cin>>s;
    int n = s.size();

    vector<int> arr(n);
    for(int i=0;i<n;i++)
    {
        arr[i] = s[i]-'a';
    }

    //build Segment Tree
    segtree.resize(4*n);
    build(0,0,n-1,arr);

    int q;
    cin>>q;
    vector<vector<int>> queries(q);
    for(int i=0;i<q;i++)
    {
        int type;
        cin>>type;
        if(type == 1){
            int x,y;
            cin>>x>>y;
            queries[i] = {x,y};
        }
        
        if (type == 2){
            int x,y,z;
            cin>>x>>y>>z;
            queries[i] = {x,y,z};
        }
    }

    for(auto &it: queries)
    {
        if(it.size() == 2)
        {
            int l = it[0];
            int idx = it[1];
            update(0,0,n-1,idx);
        }
        else
        {
            int lr = it[1];
            int rr = it[2];
            cout<<(rr-lr+1)-query(0,0,n-1,lr,rr)<<"\n";
        }
    }
}