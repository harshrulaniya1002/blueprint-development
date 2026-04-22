#include<iostream>
#include<vector>
#include<set>
#include<math.h>
#include<unordered_map>

using namespace std;

void solve(int n){
    unordered_map<int,int>mp;
    for(long long i=2;i*i<=n;i++){
        while(n%i == 0){
            mp[i]++;
            n=n/i;
        }
    }

    if(n>1){
        mp[n]++;
    }
    

    long long ans=0;
    
    while(!mp.empty()){
        long long val = 1;

        for(auto it = mp.begin(); it != mp.end(); ) {
            val *= it->first;
            it->second--;

            if(it->second == 0)
                it = mp.erase(it);  
            else
                ++it;
        }

        ans += val;
    }

    cout<<ans<<"\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin>>t;

    while(t--){
    int n;
    cin>>n;
    solve(n);
    }

}