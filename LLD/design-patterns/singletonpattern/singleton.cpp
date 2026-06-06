#include<iostream>
using namespace std;


class Singleton{
    private:
    static Singleton *instance;
    static mutex mtx; // Mutex for thread safety
    Singleton(){
        cout<< "Singleton Constructor called. New Object Created."<<endl;
    }
    public:
    /**
     * Returns the singleton instance of the Singleton class.
     *
     * This method implements the double-checked locking pattern to ensure that the Singleton instance
     * is created only once in a thread-safe manner. The first check (`if (!instance)`) is performed
     * without acquiring the mutex to avoid unnecessary locking after the instance has been initialized.
     * If the instance is not yet created, the mutex is locked to prevent race conditions between threads.
     * The second check inside the locked section ensures that no other thread has created the instance
     * in the meantime. This double-checking is necessary because multiple threads could pass the first
     * check simultaneously before any of them acquires the lock, potentially resulting in multiple
     * instances being created if only a single check was used.
     *
     * Returns:
     *   Singleton* - Pointer to the singleton instance.
     */
    static Singleton* getInstance(){
        if (!instance) {
            lock_guard<mutex> lock(mtx); // Lock the thread safety mutex
            if (!instance) { // Double-checked locking
                instance = new Singleton();
            }
        }
        return instance;
    }
};

Singleton* Singleton::instance= nullptr;
mutex Singleton::mtx;

int main(){
    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();
    if(s1 == s2)
        cout << "Both instances are the same." << endl;
    return 0;
}