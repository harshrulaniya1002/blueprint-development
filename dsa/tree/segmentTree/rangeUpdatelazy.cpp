#include<bits/stdc++.h>
using namespace std;

vector<int> segtree;
vector<int> lazy;

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


// Range Update Query in Segment Tree using Lazy Propagation
// tc -> O(logN) for updating a range in the segment tree
// sc -> O(N) for storing the lazy array
void RangeUpdate(int node, int start, int end, int l, int r, int val)
{
  if (lazy[node] != 0) {
    segtree[node] += (end-start+1) * lazy[node]; // Update it
    if (start != end) { // Not a leaf node
       lazy[2*node+1] += lazy[node]; // Mark child as lazy
       lazy[2*node+2] += lazy[node]; // Mark child as lazy
    }
    lazy[node] = 0; // Reset it
  }

  //out of range
  if(start > end || start > r || end < l) return;

  //current segment is fully in range
  if(start >= l && end <= r)
  {
    segtree[node] += (end-start+1)*val;
    if(start != end)
    {
      lazy[2*node+1] += val;
      lazy[2*node+2] += val;
    }
    return;
  }

  //partial overlap
    int mid = (start+end)/2;
    RangeUpdate(2*node+1,start,mid,l,r,val);
    RangeUpdate(2*node+2,mid+1,end,l,r,val);
    segtree[node] = segtree[2*node+1] + segtree[2*node+2];
}

void Print(vector<int>&segtree)
{
    for(auto it: segtree)
        cout<<it<<" ";
    cout<<endl;
}

int main()
{
   int n;
   cin>>n;
   vector<int> arr(n);
   for(int i=0;i<n;i++) cin>>arr[i];
   
   segtree.resize(4*n);
   lazy.resize(4*n,0);
   build(0,0,n-1,arr);

   int q;
   cin>>q;
   while(q--)
   {
      int idx,idy,val;
      cin>>idx>>idy>>val;
      RangeUpdate(0,0,n-1,idx,idy,val);
      Print(segtree);
      Print(lazy);
   }
   return 0;
}