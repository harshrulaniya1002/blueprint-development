//generate all factors of a number
#include<iostream>
#include<vector>
using namespace std;


// Time complexity: Sqrt(N)
void solve(int n)
{
    vector<int>factors;
    for(int i=1;i*i<=n;i++){
        if((n%i) == 0) {
            factors.push_back(i);
            if(i != n/i){
                factors.push_back(n/i);
            }
        }
    }

    for(auto &it : factors){
        cout<<it<<" ";
    }

    cout<<"\n";
}

int main()
{
   int n;
   cin>>n;
   solve(n);
}