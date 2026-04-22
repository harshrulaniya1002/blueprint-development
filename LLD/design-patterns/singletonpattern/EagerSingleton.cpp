#include<bits/stdc++.h>
using namespace std;


class Singleton{
    private:
    static Singleton *instance;
    Singleton(){
        cout<< "Singleton Constructor called. New Object Created."<<endl;
    }
    public:
    static Singleton* getInstance(){
        return instance;
    }
};

// What is Eager Initialization?
// Eager Initialization means the singleton instance is created at the time of program startup, before any method (like getInstance()) is called.
// This is in contrast to Lazy Initialization, where the instance is created only when it is first needed.

// ------------------------------Drawbacks of Eager Initialization--------------------------------
// Resource Wastage:
// The instance is created even if it is never used. This can waste memory and resources, especially if the singleton is heavy.

// No Exception Handling:
// If the constructor throws an exception, it happens at program startup, which can be hard to handle.

// Lack of Flexibility:
// You cannot pass parameters to the constructor, since the instance is created before any method is called.

// Order of Initialization Issues:
// In complex projects with multiple translation units, the order of static initialization is not guaranteed across files, which can lead to subtle bugs (the "static initialization order fiasco").
Singleton* Singleton::instance= new Singleton();

int main(){
    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();
    if(s1 == s2)
        cout << "Both instances are the same." << endl;
    return 0;
}