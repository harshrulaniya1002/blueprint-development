#include <iostream>

// Base Class
class Person {
protected:
    // Accessible by derived classes, but private to the outside world
    int age; 

public:
    Person(int a) : age(a) {}
    
    void display_age() const {
        std::cout << "Person's age is: " << age << std::endl;
    }
};

// Derived Class 1
class Student : public Person {
public:
    Student(int a) : Person(a) {}

    // Directly accessing and modifying the protected member 'age'
    void celebrate_birthday() {
        age++; 
        std::cout << "Student celebrated birthday. New age: " << age << std::endl;
    }
};

// Derived Class 2
class Teacher : public Person {
public:
    Teacher(int a) : Person(a) {}

    void set_age(int new_age) {
        age = new_age; // Direct modification is allowed
        std::cout << "Teacher age set to: " << age << std::endl;
    }
};

int main() {
    Student s(19);
    Teacher t(40);
    
    s.display_age();
    s.celebrate_birthday();

    t.display_age();
    t.set_age(41);
    
    // Compilation Error Examples:
    // s.age = 20; // ERROR: 'int Person::age' is protected
    // std::cout << t.age; // ERROR: 'int Person::age' is protected

    return 0;
}