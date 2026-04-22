
#include<iostream>
#include<mutex>
#include<vector>
#include<shared_mutex>

using namespace std;

// race condition
// when data is shared among many threads, and they are accessing at same __TIME__
// 1) lock_guard -> poora function par control rehta, function khatam hone par automatically lock release hoga
//    declaration: lock_guard<mutex> lock(mtx);

// 2) unique_lock -> humhare pass control rehta kab unlock karna
//    declaration: unique_guard<mutex> lock(mtx);

// try to add mutex only on that section which is actually critical ,
// otherwise code will became synchronized and mulit-threading is not that useful


//solve
// using mutexes 
// Mutual exclusion algorithms prevent multiple threads from simultaneously accessing shared resources. 
// This prevents data races and provides support for synchronization between threads

mutex mtx;
shared_mutex s_mtx;

int vari = 0;

void func1(){
 lock_guard<mutex> lock(mtx);
 vari++;
 cout<<1<<" "<<vari<<"\n";
}

void func2(){
 lock_guard<mutex> lock(mtx);
 vari--;
 cout<<2<<" "<<vari<<"\n";
}

int main()
{
    thread t1(func1);
    thread t2(func2);
    t1.join();
    t2.join();
}



// for read heavy application like DNS (many reads, few writes)
//here also read and write can be simulatenously on dnsCache Map, so we add the locks

// but as we added locks on dnsRead, and on Application is read heavy so , so when one thread is reading, other have to wait 
// so purpose of multi-threading in not working, all threds are not reading parallely.

// void dnsWrite(int ip, string name){
    // lock_guard<mutex> lock(mtx);
//     dnsCache[name] = ip;
// }

// int dnsRead(string name){
//     lock_guard<mutex> lock(mtx);
//     return dnsCache[name];
// }

//Solution:
// use shared lock 
// multi-write request will not happen because of lock_guard, but multiple read can be possible because of shared_lock
void dnsWrite(int ip, string name){
    lock_guard<shared_mutex> lock(s_mtx);
    dnsCache[name] = ip;
}

int dnsRead(string name){
    shared_lock<shared_mutex> lock(s_mtx);
    return dnsCache[name];
}
