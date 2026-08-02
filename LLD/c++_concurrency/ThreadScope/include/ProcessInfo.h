#ifndef PROCESS_INFO.h
#define PROCESS_INFO.h

class ProcessInfo{
    public:
    int pid;
    static ProcessInfo capture();
};


#endif