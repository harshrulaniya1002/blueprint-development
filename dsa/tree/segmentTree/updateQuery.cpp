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

// Point Update Query in Segment Tree
// tc -> O(logN) for updating a value in the segment tree
// sc -> O(1) no extra space is required
void update(int idx, int val, int node, int l, int r)
{
  if(l == r){
    segtree[node] = val;
    return;
  }
  int mid = (l+r)/2;
  if(idx <= mid){
    update(idx,val,2*node+1,l,mid);
  }
  else{
    update(idx,val,2*node+2,mid+1,r);
  }
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
   build(0,0,n-1,arr);

   int q;
   cin>>q;
   while(q--)
   {
      int idx,val;
      cin>>idx>>val;
      update(idx,val,0,0,n-1);
      Print(segtree);
   }
   return 0;
}