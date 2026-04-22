#include<iostream>

using namespace std;

template <typename T>

class UniquePtr {
    private:
    T* ptr;

    public:
        // Constructor
    UniquePtr(T* p = nullptr) : ptr(p) {
        cout<<"UniquePtr constructor called. Allocating memory for value: " << *ptr << endl;
    }

    UniquePtr(const UniquePtr<T>& ptr) = delete; // Disable copy constructor
    UniquePtr& operator=(const UniquePtr<T>& ptr) = delete; // Disable copy assignment operator

    // Move constructor
    UniquePtr(UniquePtr<T>&& other) : ptr(other.ptr) {
        cout<<"UniquePtr move constructor called. Transferring ownership of value: " << *ptr << endl;
        other.ptr = nullptr; // Release ownership from the source
    }

    // Move assignment operator
    UniquePtr& operator=(UniquePtr<T>&& other) {
        cout<<"UniquePtr move assignment operator called. Transferring ownership of value: " << *other.ptr << endl;
        if (this != &other) {
            delete ptr; // Free existing resource
            ptr = other.ptr; // Transfer ownership
            other.ptr = nullptr; // Release ownership from the source
        }
        return *this;
    }

    //operator overloading for dereference
    T* operator->() {
        return ptr;
    }

    // operator overloading for dereference
    T& operator*() {
        return *ptr;
    }

    //getter for raw pointer (if needed)
    T* get() const {
        return ptr;
    }

    //reset function to release ownership and optionally take a new pointer
    void reset(T* p = nullptr) {
        cout<<"UniquePtr reset called. Releasing ownership of value: " << (ptr ? to_string(*ptr) : "null") << endl;
        delete ptr; // Free existing resource
        ptr = p; // Take ownership of new resource
    }

    ~UniquePtr() {
        cout<<"UniquePtr destructor called. Freeing memory for value: " << (ptr ? to_string(*ptr) : "null") << endl;
        delete ptr; // Automatically free memory when object goes out of scope
    }
};

int main(){
    UniquePtr<int> ptr1(new int(10)); // UniquePtr takes ownership of the integer
    // UniquePtr<int> ptr2 = ptr1; // Error: Copying is not allowed for UniquePtr
    UniquePtr<int> ptr3(new int(20)); // Another UniquePtr
    // UniquePtr<int> ptr4 = ptr3; // Error: Copying is not allowed for UniquePtr
    UniquePtr<int> ptr5 = move(ptr1); // Transfer ownership from ptr1 to ptr5
    cout<<ptr3.get()<<endl; // Output: 10
    ptr1.reset(); // Release ownership of the integer, ptr1 becomes empty
    cout<<ptr1.get()<<endl; // Output: nullptr
}