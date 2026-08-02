#ifndef THREAD_INFO.h
#define THREAD_INFO.h
#include<thread>
using namespace std;

class ThreadInfo{
    public:
    int logicalId;
    thread::id cppId;
    pthread_t nativeId;
    void* stackAddress;
    string state;
};


#endif