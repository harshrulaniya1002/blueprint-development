//generating all prime factors using trial division approach (repetitive factors also: (eg: 24 -> (2,2,2,3)))

// The above method works with the observation that if we iterate
// till sqrt(N), we will be able to generate all the factors of that
// number.


#include<iostream>
#include<vector>
using namespace std;


// Time complexity: Sqrt(N)
void solve(int n)
{
    vector<int>prime_factors;
    for(int i=2;i*i<=n;i++){
        while(n%i == 0){
            prime_factors.push_back(i); 
            n = n/i;
        }
    }
    
    if(n > 1) {
        prime_factors.push_back(n);
    }

    for(auto &it : prime_factors){
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