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

int main()
{
   int n;
   cin>>n;
   vector<int> arr(n);
   for(int i=0;i<n;i++) cin>>arr[i];
   segtree.resize(4*n);
   build(0,0,n-1,arr);

   //print the segment tree
    for(int i=0;i<4*n;i++)
    {
         cout<<segtree[i]<<" ";
    }
   return 0;
}