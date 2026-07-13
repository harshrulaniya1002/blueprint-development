#include<iostream>
using namespace std;


//tc -> Amortized: O(α(n)) <= 4(inverse ackermann function) (exteremely slow growing)
// even if n ~~ 10^18 -> α(n) ≈ 4
class DSU{
    vector<int>parent;
    vector<int>rank;
    vector<int>size;

    public:

    void make_set(int n){
        parent.resize(n+1);
        rank.resize(n+1,0);
        size.resize(n+1,1);

        for(int i=0;i<=n;i++){
            parent[i]=i;
        }
    }

    int findParent(int x){
        if(x==parent[x]){
            return x;
        }

        return parent[x] = findParent(parent[x]);
    }

    void UnionByRank(int x, int y){
        int pu = findParent(x);
        int pv = findParent(y);
        if(pu == pv){
            return ;
        }

        if(rank[pu] < rank[pv]){
            parent[pu] = pv;
        }else if(rank[pv] < rank[pu]){
            parent[pv] = pu;
        }else{
            parent[pv] = pu;
            rank[pu]++;
        }
    }

    void UnionBySize(int x, int y){
        int pu = findParent(x);
        int pv = findParent(y);
        if(pu == pv){
            return ;
        }

        if(size[pu] < size[pv]){
            parent[pu] = pv;
            size[pv] += size[pu];
        }else{
            parent[pv] = pu;
            size[pu] += size[pv];
        }
    }
};

int main(){
    DSU ds;
    ds.make_set(7);
    // ds.UnionByRank(1,2);
    // ds.UnionByRank(2,3);
    // ds.UnionByRank(4,5);
    // ds.UnionByRank(6,7);
    // ds.UnionByRank(5,6);

    ds.UnionBySize(1,2);
    ds.UnionBySize(2,3);
    ds.UnionBySize(4,5);
    ds.UnionBySize(6,7);
    ds.UnionBySize(5,6);

    if(ds.findParent(3) == ds.findParent(7)){
        cout<<"They belong to same component"<<"\n";
    }else{
        cout<<"They are in different components"<<"\n";
    }

    ds.UnionBySize(3,7);
    if(ds.findParent(3) == ds.findParent(7)){
        cout<<"They belong to same component"<<"\n";
    }else{
        cout<<"They are in different components"<<"\n";
    }

}