// The Sieve of Eratosthenes is one of the most efficient ways to find
// all primes smaller than or equal to the number N.

// Through this method we can precompute all prime numbers less
// than or equal to 1e7.

#include<iostream>
#include<vector>
using namespace std;


vector<bool> isPrime;
// Time complexity: N*Log(Log(N))
void solve(int n)
{
    for(long long i=2;i*i<=n;i++){
        if(isPrime[i]){
            for(long long j=i*i; j<=n; j+= i){
                isPrime[j]=0;
            }
        }
    }

    int countPrime=0;
    for(int i=0; i<=n ;i++){
        if(isPrime[i]){
            countPrime++;
        }
    }

    cout<<countPrime<<"\n";
}

int main()
{
   int n;
   cin>>n;
   isPrime.resize(n+1,1);
   isPrime[0] = isPrime[1] = 0;
   solve(n);
}