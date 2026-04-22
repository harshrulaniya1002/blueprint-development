#include<iostream>
#include<vector>
using namespace std;

vector<bool>isPrime;
vector<int>primes;

int next_prime(int num){
    int i=0;
    int j=primes.size();
    int nearest_prime=1e5+4;
    while(i<=j){
        int mid = i+(j-i)/2;
        if(primes[mid] > num){
            nearest_prime = min(nearest_prime,primes[mid]);
            j=mid-1;
        }else if(mid < num){
            i=mid+1;
        }else{
            return mid;
        }
    }

    return nearest_prime;
}

void solve(int n, int m, vector<vector<int>>&mat){

    int maxPrimesRow=1e7;
    int maxPrimeCol=1e7;

    for(int i=0;i<n;i++){
        int countmoves=0;
        for(int j=0;j<m;j++){
            if(!isPrime[mat[i][j]]){
                int prime_greater_than_matij = next_prime(mat[i][j]);
                countmoves += (prime_greater_than_matij-mat[i][j]);
            }
        }

        maxPrimesRow = min(countmoves,maxPrimesRow);
    }


    for(int i=0;i<m;i++){
        int countmoves=0;
        for(int j=0;j<n;j++){
            if(!isPrime[mat[j][i]]==1) {
                int prime_greater_than_matij = next_prime(mat[j][i]);
                countmoves += (prime_greater_than_matij-mat[j][i]);
            }
        }

        maxPrimeCol = min(countmoves,maxPrimeCol);
    }

    cout<<min(maxPrimesRow,maxPrimeCol)<<"\n";
}

int main()
{
    int n,m;
    cin>>n>>m;

    isPrime.resize(1e5+4,1);
    isPrime[0]=isPrime[1]=0;
    for(long long i=2; i<=(1e5+4); i++){
        if(isPrime[i]){
            primes.push_back(i);
            for(long long j=i*i; j<=(1e5+4); j+=i){
                isPrime[j]=0;
            }
        }
    }

    vector<vector<int>>mat(n,vector<int>(m));
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            cin>>mat[i][j];
        }
    }

    solve(n,m,mat);
}