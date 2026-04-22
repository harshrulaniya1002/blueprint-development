// SPF of a number N is the smallest prime number that divides that
// number N.

// SPF of a prime number N is the number itself.
// By using Sieve of Eratosthenes we can precompute SPF of
// numbers upto N.
// Time Complexity : N * log (logN)


#include<iostream>
#include<vector>
using namespace std;


vector<int> spf;
// Time complexity: N*Log(Log(N))
void solve(int n)
{
    for(long long i=2;i*i<=n;i++){
        if(spf[i] == i){
            for(long long j=i*i; j<=n; j+= i){
                if(spf[j] == j){
                    spf[j] = i;
                }
            }
        }
    }

    for(int i=0; i<=n ;i++){
        cout<<spf[i]<<" ";
    }

    cout<<"\n";
}

int main()
{
   int n;
   cin>>n;
   spf.resize(n+1);
   for(int i=0;i<=n;i++){
    spf[i]=i;
   }

   solve(n);
}