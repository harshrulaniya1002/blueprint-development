#include<bits/stdc++.h>
using namespace std;

//brute force
//use a vector<pair<int,int>> , keep recently use at last and remove the first ele if capacity exceeds

//optimal approach
class LRUCache {
public:
    list<int>dll;
    map<int,pair<list<int>::iterator,int>>mp;
    int n;
    LRUCache(int capacity) {
        n=capacity;
    }

    void makeRecentlyUsed(int key){

        //delete the key from address stored in map
        dll.erase(mp[key].first);

        //add in front
        dll.push_front(key);

        //update the addres of key in map
        mp[key].first = dll.begin();
    }
    
    int get(int key) {
        if(mp.find(key) != mp.end()){
            int value = mp[key].second;
            makeRecentlyUsed(key);
            return value;
        }

        return -1;
    }
    
    void put(int key, int value) {
        if(mp.find(key) != mp.end()){
            mp[key].second = value;
            makeRecentlyUsed(key);
        }else{
            dll.push_front(key);
            mp[key] = {dll.begin(),value};
            n--;
        }

        if(n<0){
            mp.erase(dll.back());
            dll.pop_back();
            n++;
        }
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */