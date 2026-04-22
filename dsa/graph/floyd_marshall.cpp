#include<bits/stdc++.h>
using namespace std;

int n;
vector<vector<int>>mat;

//time complexity O(V*V*V) , E is no of edges
void Floyd_Marshall(){
  
    vector<vector<int>>cost(n,vector<int>(n,INT_MAX));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(mat[i][j] != -1){
                cost[i][j]=mat[i][j];
            }
        }
        mat[i][i]=0;
    }


    for(int k=0;k<n;k++){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(cost[i][k] != INT_MAX && cost[k][j] != INT_MAX){
                    cost[i][j] = min(cost[i][j] , cost[i][k] + cost[k][j]);
                }
            }
        }
    }

    for(int i=0;i<n;i++){
        if(cost[i][i] < 0){
            cout<<"NEGATIVE CYCLE EXITS"<<"\n";
            return ;
        }
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(cost[i][j] == INT_MAX){
                cout<<-1<<" ";
            }else{
                cout<<cost[i][j]<<" ";
            }
        }
        cout<<"\n";
    }
}

int main()
{
    cin>>n;
    mat.resize(n,vector<int>(n));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>mat[i][j];
        }
    }

    Floyd_Marshall();
}