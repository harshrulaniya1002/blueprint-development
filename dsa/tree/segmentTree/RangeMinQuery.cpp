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
    segtree[node] = min(segtree[2*node+1], segtree[2*node+2]);

}

// TC -> O(LogN) for range min query
// SC -> O(1) no extra space is required
int rangeMin(int node, int start, int end, int ql, int qr)
{
  if(ql > end || qr < start) return INT_MAX; // no overlap
  if(ql <= start && qr >= end) return segtree[node]; // complete overlap
  else{
    int mid = (start+end)/2;
    int left = rangeMin(2*node+1,start,mid,ql,qr);
    int right = rangeMin(2*node+2,mid+1,end,ql,qr);
    return min(left,right);
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
      cout<<rangeMin(0,0,n-1,idx,idy)<<endl;
   }
   return 0;
}