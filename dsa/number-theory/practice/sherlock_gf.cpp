#include<iostream>
#include<vector>
using namespace std;

//codeforces: Almost Prime
vector<bool> isPrime;
void solve(int n)
{
    if(n==1){
        cout<<1<<"\n"<<1<<"\n";
        return;
    }
    int one=0;
    int two=0;
    int count=1;
    for(int i=2;i< (n+2);i++){
        if(isPrime[i]==0) one++;
        if(isPrime[i]==1) two++;
        if((one > 0) && (two>0)) {
            count=2;
            break;
        }
    }

    cout<<count<<"\n";
    for(int i=2;i< (n+2);i++){
        cout<<isPrime[i]+1<<" ";
    }

    cout<<"\n";
}

int main()
{
    int n;
    cin>>n;
    isPrime.resize(n+3,0);
    isPrime[0] = isPrime[1] = 1;
    for(long long i=2; i*i <= (n+2) ; i++){
        if(isPrime[i]==0){
            for(long long j=i*i ; j<=(n+2) ;j += i){
                isPrime[j]=1;
            }
        }
    }
    solve(n);
}
