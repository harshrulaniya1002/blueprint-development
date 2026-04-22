#include<iostream>
#include<mutex>
using namespace std;

//we have to print FooBar n times, even though you have 2 threads for 2 different functions , which prints in random order,
// so you have to make this threads synchonized : t1 -> t2 (order)
// we use , locks , condition variables.


class FooBar{
    private:
      int n;
      bool isFooPrinted;
      condition_variable cv;
      mutex mtx;
    public:
      FooBar(int n){
        this->n = n;
        isFooPrinted = false;
      }
      
      void Foo(){
        for(int i=0;i<n;i++){
            unique_lock<mutex> lock(mtx);
            cv.wait(lock,[this]{return !isFooPrinted;});
            cout<<"Foo"<<" ";
            isFooPrinted=true;
            cv.notify_all();
        }
      }

      void Bar(){
        for(int i=0;i<n;i++){
            unique_lock<mutex> lock(mtx);
            cv.wait(lock,[this]{return isFooPrinted;});
            cout<<"Bar"<<"\n";
            isFooPrinted=false;
            cv.notify_all();
        }
      }
};


int main()
{
    FooBar fb(5);
    thread t1(fb.Foo)
    thread t2(fb.Bar)
    t1.join();
    t2.join();
}