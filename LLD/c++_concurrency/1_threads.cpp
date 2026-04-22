#include<iostream>
#include<thread>
#include<mutex>
#include<vector>

using namespace std;
// https://en.cppreference.com/w/cpp/atomic.html
mutex mtx;

void threadFunction(int a)
{
    
    cout << "Thread running with id = " << this_thread::get_id() << " is running" << a <<"\n";
}

int main()
{
  cout<<"actual concurrency cores = "<<thread::hardware_concurrency()<<"\n";
  cout<<"main thread with Id "<<this_thread::get_id()<<endl;
  
  thread t(threadFunction,10);
  t.join();

  vector<thread>threads;
  for(int i=0;i<5;i++){
    threads.push_back(thread(threadFunction,i));
  }

  for(int i=0;i<5;i++){
    threads[i].join();
  }

  return 0;
}