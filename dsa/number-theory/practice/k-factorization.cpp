#include<iostream>
#include<vector>
using namespace std;


void solve(int n, int k){

   vector<int>factors;
   for(long long i=2;i*i<=n;i++){
     while(n%i == 0){
        factors.push_back(i);
        n=n/i;
     }
   }

   if(n>1) factors.push_back(n);

   if(k > factors.size()){
    cout<<-1<<"\n";
    return;
   }

   for(int i=0;i<k-1;i++){
    cout<<factors[i]<<" ";
   }

   long long product=1;
   for(int i=k-1;i<factors.size();i++){
    product = product*(1LL*factors[i]);
   }

   cout<<product<<"\n";
}

int main()
{
    int n,k;
    cin>>n>>k;
    solve(n,k);
}