// SPF of number can be used to calculate prime factorization in
// more efficient way as compared to trial division method.

// Time Complexity :
// Precomputation time : N * log ( log (N) )
// Query time : O(logN)


#include<iostream>
#include<vector>
using namespace std;


vector<int> spf;
// Time complexity: N*Log(Log(N))

void solve(int n)
{
    vector<pair<int,int>>prime_factors;
    while(n != 1){
        int prime = spf[n];
        int cnt=0;
        while((n % prime) == 0){
            cnt++;
            n = n/prime;
        }
        prime_factors.push_back({prime,cnt});
    }

    for(auto &pr : prime_factors){
        cout<<pr.first<<"-> "<<pr.second<<"\n";
    }
}

int main()
{
   int n;
   cin>>n;

   //precomputation of spf
   spf.resize(n+1);
   for(int i=0;i<=n;i++){
    spf[i]=i;
   }

    for(long long i=2;i*i<=n;i++){
        if(spf[i] == i){
            for(long long j=i*i; j<=n; j+= i){
                if(spf[j] == j){
                    spf[j] = i;
                }
            }
        }
    }


   solve(n);
}