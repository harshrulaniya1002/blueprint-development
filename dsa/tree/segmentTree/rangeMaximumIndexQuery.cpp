#include<bits/stdc++.h>
using namespace std;

vector<int> segtree;

// tc -> O(N) for building the segment tree
// sc -> O(N) for storing the segment tree
void build(int node, int l, int r, vector<int> &arr)
{   
    if(l == r)
    { 
        // storing index of the maximum element
        segtree[node] = l;
        return;
    }
    int mid = (l+r)/2;
    build(2*node+1,l,mid,arr);
    build(2*node+2,mid+1,r,arr);
    
    // storing index of the maximum element
    int leftIndex = segtree[2*node+1];
    int rightIndex = segtree[2*node+2];
    if(arr[leftIndex] >= arr[rightIndex])
        segtree[node] = leftIndex;
    else
        segtree[node] = rightIndex;    
}


// TC -> O(LogN) for range max index query
// SC -> O(1) no extra space is required
int rangeMaxIndex(int node, int start, int end, int ql, int qr, vector<int>& arr)
{
  if(ql > end || qr < start) return -1; // no overlap
  if(ql <= start && qr >= end) return segtree[node]; // complete overlap
  else{
    int mid = (start+end)/2;
    int leftIndex = rangeMaxIndex(2*node+1,start,mid,ql,qr,arr);
    int rightIndex = rangeMaxIndex(2*node+2,mid+1,end,ql,qr,arr);
    
    if(leftIndex == -1) return rightIndex;
    if(rightIndex == -1) return leftIndex;
    
    if(arr[leftIndex] >= arr[rightIndex])
        return leftIndex;
    else
        return rightIndex;    
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
      cout<<rangeMaxIndex(0,0,n-1,idx,idy,arr)<<endl;
   }
   return 0;
}