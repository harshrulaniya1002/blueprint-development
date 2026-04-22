#include<iostream>
#include<vector>
#include<set>
using namespace std;

vector<bool>isPrime;
set<long long int>st;

void solve(int n, vector<long long>&arr){
    for(int i=0;i<n;i++){
        if(st.find(arr[i]) != st.end()){
            cout<<"YES"<<"\n";
        }else{
            cout<<"NO"<<"\n";
        }
    }

    return ;
}

int main()
{
    int n;
    cin>>n;
    isPrime.resize(1e6+1,1);

    isPrime[0]=isPrime[1]=0;
    for(long long i=2;i*i<=1e6;i++){
        if(isPrime[i]){
            for(long long j=i*i; j<=1e6;j+= i){
                isPrime[j]=0;
            }
        }
    }

    for(int i=2;i<=1e6;i++) {
        if(isPrime[i]) st.insert(1LL*i*i);
    }

    vector<long long> arr(n);
    for(int i=0;i<n;i++) cin>>arr[i];
    solve(n,arr);
}