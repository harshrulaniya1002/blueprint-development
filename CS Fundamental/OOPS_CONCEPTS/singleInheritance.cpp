#include<iostream>
using namespace std;

class Animal{
    public:
    void eat(){
        cout<<"Animal is eating"<<"\n";
    }
};

class Dog : public Animal {
    public:

    void eat(){
        cout<<"dog is chewing bones"<<"\n";
    }

    void bark(){
        cout<<"woof woof"<<"\n";
    }
};


int main(){
    Dog d;
    d.Animal::eat();
    d.eat();
    d.bark();
}