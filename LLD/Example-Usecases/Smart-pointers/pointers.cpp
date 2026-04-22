#include<iostream>
using namespace std;

// RAII -> resource acquisition is initialization
//     - A programming idiom that ensures resource management (like memory, file handles, locks) is tied to the lifetime of objects.
//     - When an object is created, it acquires the resource; when the object goes out of scope, it automatically releases the resource.

// void func(){
//     for(int i=0;i<1000000;i++){
//         cout<<"testing"<<"\n";
//         int *a = new int(10); //allocation in heap
//         delete a; //freeing memory
//     }
// }


class Wrapper {
public:
    int* ptr;

    Wrapper(int value) {
        cout<<"Wrapper constructor called. Allocating memory for value: " << value << endl;
        ptr = new int(value); // Allocate memory
    }

    ~Wrapper() {
        cout<<"Wrapper destructor called. Freeing memory for value: " << *ptr << endl;
        delete ptr; // Automatically free memory when object goes out of scope
    }
};


int main()
{
    Wrapper w(10);
    return 0;
}