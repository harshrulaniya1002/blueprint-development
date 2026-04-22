#include<iostream>
using namespace std;

//codeforces: Almost Prime

void solve(int n)
{
    int count=0;
    for(int i=1;i<=n;i++)
    {
        int num = i;
        int cnt=0;
        for(int j=2;j*j <= n; j++){
            bool once = false;
            while(num%j == 0){
                if(!once) {
                    cnt++; once=true;
                }
                num = num/j;
            }
        }
        if(num > 1) cnt++;
        if(cnt==2) count++;
    }

    cout<<count<<"\n";
}

int main()
{
    int n;
    cin>>n;
    solve(n);
}
