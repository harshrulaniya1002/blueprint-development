#include<bits/stdc++.h>
using namespace std;

vector<int> segtree;

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
   build(0,0,n-1,arr);

   int q;
   cin>>q;
   while(q--)
   {
      int idx,idy;
      cin>>idx>>idy;
      cout<<rangeSum(0,0,n-1,idx,idy)<<endl;
   }
   return 0;
}