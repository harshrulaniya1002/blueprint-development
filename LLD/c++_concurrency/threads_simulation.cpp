#include<iostream>
#include<thread>
#include<chrono>
#include<unistd.h>
using namespace std;


#include <mutex>

mutex coutMutex;

void func1(int i)
{
    {
        lock_guard<mutex> lock(coutMutex);
        cout << "Worker " << i
             << " | Thread ID = "
             << this_thread::get_id() << endl;
    }


    // not a critical section, so out of lock_guard
    this_thread::sleep_for(chrono::seconds(60));

    {
        lock_guard<mutex> lock(coutMutex);
        cout << "Worker " << i << " finished\n";
    }
}


int main(){
    int n;
    cin >> n;

    cout << "=========================\n";
    cout << "Process ID : " << getpid() << endl;
    cout << "Main Thread: " << this_thread::get_id() << endl;
    cout << "=========================\n\n";

    vector<thread> threads;
    for (int i = 0; i < n; i++) {
        threads.emplace_back(func1, i);
    }

    cout << "\nAll threads created.\n";
    cout << "Open Activity Monitor or another terminal now.\n";
    cout << "Press ENTER to join all threads...\n";

    cin.ignore();
    cin.get();

    for (auto &t : threads)
        t.join();
    cout << "\nAll threads joined.\n";
}