🚀 Object-Oriented Programming (OOP) in C++ — Complete Notes & Concepts

A comprehensive guide to Object-Oriented Programming in C++ with explanations, relationships, polymorphism, memory management, and design principles.

⸻

📖 Table of Contents

1. Introduction to OOP
2. Classes and Objects
3. Encapsulation and Data Hiding
4. Constructors and Constructor Overloading
5. Destructors and RAII
6. Copy Constructor and Deep Copy
7. Initialization Lists
8. Static Members
9. Friend Functions and Friend Classes
10. Inheritance
11. Types of Inheritance
12. Access Specifiers
13. Constructor Chaining
14. Polymorphism
15. Compile-Time Polymorphism
16. Runtime Polymorphism
17. Virtual Functions
18. Pure Virtual Functions and Abstract Classes
19. Virtual Destructors
20. Operator Overloading
21. Composition vs Aggregation
22. Association Relationships
23. SOLID Principles
24. Rule of Three / Five / Zero
25. Move Semantics
26. RTTI and Dynamic Casting
27. Templates and Generic Programming
28. Smart Pointers
29. Exception Safety
30. Summary

⸻

1. Introduction to OOP

Object-Oriented Programming (OOP) organizes programs around objects, which encapsulate both data and behavior.

Four Pillars of OOP

* Encapsulation
* Abstraction
* Inheritance
* Polymorphism

Benefits:

✔ Modularity
✔ Reusability
✔ Maintainability
✔ Scalability
✔ Better Code Organization

⸻

2. Classes and Objects

A class acts as a blueprint, while objects are instances of that class.

class Rectangle {
private:
    int length, width;
public:
    int area() {
        return length * width;
    }
};

Object creation:

Rectangle r;

⸻

3. Encapsulation & Data Hiding

Encapsulation combines data and methods into one unit and controls access using access specifiers.

class Student {
private:
    int marks;
public:
    void setMarks(int m) {
        marks = m;
    }
    int getMarks() const {
        return marks;
    }
};

Benefits:

* Protects data
* Prevents invalid states
* Improves maintainability
* Provides controlled access

⸻

4. Constructors

Constructors initialize objects.

Default Constructor

Rectangle() {}

Parameterized Constructor

Rectangle(int l, int w) : length(l), width(w) {}

Constructor Overloading

Rectangle();
Rectangle(int l, int w);

This is an example of Compile-Time Polymorphism.

⸻

5. Destructors and RAII

Destructor:

~Rectangle() {}

Called automatically when object lifetime ends.

RAII (Resource Acquisition Is Initialization):

Resource lifetime = Object lifetime.

Prevents:

* Memory leaks
* File leaks
* Resource exhaustion

⸻

6. Copy Constructor

Point(const Point& other)
{
    x = other.x;
    y = other.y;
}

Invoked during:

Point p2 = p1;

Default behavior:

Shallow Copy

Copies member values.

Custom implementation enables:

Deep Copy

Necessary when managing:

* Dynamic memory
* Files
* Sockets

Prevents:

* Double deletion
* Shared memory bugs

⸻

7. Initialization Lists

Rectangle(int l,int w):length(l),width(w){}

Advantages:

Efficiency

Direct initialization avoids default construction + assignment.

Required For

const members

const int id;

References

int& ref;

Base classes

Derived():Base(){}

⸻

8. Const Member Functions

int getAge() const;

Guarantees that member variables cannot be modified.

Inside a const function:

age = 20; // ❌ Error

Function parameter const:

void fun(const int x)

prevents modifying parameter x only.

⸻

9. Static Members

Static variables belong to the class rather than objects.

class BankAccount {
private:
    static int total_accounts;
};

Shared among all objects.

Static Member Function

static int getTotal();

Can access:

✔ Static variables

Cannot access:

❌ Non-static members

Because static functions do not have a this pointer.

⸻

10. Friend Functions

friend int calculate_volume(Box&);

Allows external functions to access private members.

Used for:

* Operator overloading
* Utility functions

⸻

11. Friend Classes

friend class Inspector;

All member functions of the friend class gain access to private members.

Useful for:

* Iterators
* Testing
* Helper classes

⸻

12. Inheritance

Represents an IS-A relationship.

class Dog : public Animal {};

Dog IS-A Animal.

Benefits:

* Code reuse
* Hierarchy creation
* Extensibility

⸻

13. Types of Inheritance

Single Inheritance

Dog → Animal

⸻

Multilevel Inheritance

Vehicle
   ↓
Car
   ↓
SportsCar

⸻

Multiple Inheritance

class C : public A, public B {};

⸻

Hierarchical Inheritance

Animal
├── Dog
├── Cat
└── Cow

⸻

Hybrid Inheritance

Combination of multiple forms.

⸻

14. Access Specifiers

Private

Accessible only inside class.

private:

⸻

Protected

Accessible inside derived classes.

protected:

⸻

Public

Accessible everywhere.

public:

⸻

15. Constructor Chaining

Order:

Base Constructor
↓
Derived Constructor

Destruction order:

Derived Destructor
↓
Base Destructor

⸻

16. Polymorphism

Polymorphism means many forms.

Two types:

Compile-Time Polymorphism

Resolved by compiler.

Runtime Polymorphism

Resolved during execution.

⸻

17. Compile-Time Polymorphism

Function Overloading

print(int);
print(double);

⸻

Constructor Overloading

Rectangle();
Rectangle(int,int);

⸻

Operator Overloading

Point operator+(const Point&);

Benefits:

* Better readability
* Mathematical representation

⸻

18. Runtime Polymorphism

Achieved through:

Virtual Functions

virtual double area();

Base pointer:

Shape* ptr;

Actual function decided at runtime.

Implemented using:

vtable (Virtual Table)

and

vptr (Virtual Pointer)

Dynamic binding occurs during execution.

⸻

19. Virtual Functions

Without virtual:

Shape* ptr = new Circle();
ptr->area();

Calls:

Shape::area()

With virtual:

Calls:

Circle::area()

Enables:

* Dynamic dispatch
* Runtime polymorphism

⸻

20. Pure Virtual Functions

virtual double area() = 0;

Makes class abstract.

class Shape
{
public:
    virtual double area() = 0;
};

Cannot instantiate:

Shape s; // ❌

Derived classes must override.

⸻

21. Abstract Classes

Act as interfaces or blueprints.

Used to enforce common behavior.

Example:

Shape
  |
------------
|          |
Circle   Square

⸻

22. Virtual Destructor

Always use virtual destructors when base class pointers delete derived objects.

virtual ~Shape(){}

Without virtual:

delete ptr;

Calls only:

Base::~Base()

Result:

❌ Memory leak

With virtual:

Derived Destructor
↓
Base Destructor

⸻

23. Operator Overloading

Example:

Point operator+(const Point& other)

Common operators:

* ￼	
* ￼	
* ￼	
* ==
* []
* <<

Enhances readability.

⸻

24. Subscript Operator []

int& operator[](int index)

Allows:

arr[2] = 10;

Returning reference enables assignment.

Boundary checks improve safety.

⸻

25. Association Relationships

IS-A Relationship

Inheritance

Dog IS-A Animal

⸻

HAS-A Relationship

Composition or Aggregation

Car HAS-A Engine

⸻

USES-A Relationship

Dependency

Printer uses Paper

Temporary relationship.

⸻

26. Composition

Strong HAS-A relationship.

class Car
{
    Engine engine;
};

Characteristics:

✔ Ownership

✔ Same lifetime

✔ Automatic destruction

Example:

Car owns Engine

⸻

27. Aggregation

Weak HAS-A relationship.

class Department
{
    vector<Employee*> employees;
};

Characteristics:

✔ Independent lifetimes

✔ Shared objects

✔ No ownership

⸻

28. Dependency Relationship

Weakest relationship.

Temporary interaction.

void print(Document&);

Printer depends on Document.

⸻

29. Rule of Three

If a class defines one of:

1. Destructor
2. Copy Constructor
3. Copy Assignment Operator

It likely needs all three.

⸻

30. Rule of Five

Adds:

4. Move Constructor
5. Move Assignment Operator

Used for efficient resource transfer.

⸻

31. Move Semantics

Introduced in C++11.

Move Constructor:

String(String&& other)

Transfers ownership instead of copying.

Benefits:

✔ Faster

✔ Avoids unnecessary copies

⸻

32. Rule of Zero

Prefer standard containers and smart pointers.

Then:

No custom destructor

No copy constructor

No move constructor

Compiler-generated functions are sufficient.

⸻

33. RTTI (Run-Time Type Information)

dynamic_cast

Safe downcasting.

Dog* d = dynamic_cast<Dog*>(animalPtr);

typeid

Returns actual type.

typeid(*ptr).name();

⸻

34. Templates (Generic Programming)

template<typename T>
T max(T a,T b)
{
    return a>b?a:b;
}

Provides:

* Code reuse
* Type independence
* Compile-time optimization

⸻

35. Smart Pointers

unique_ptr

Single ownership.

unique_ptr<int> ptr;

⸻

shared_ptr

Shared ownership.

Reference counting.

⸻

weak_ptr

Prevents circular references.

⸻

36. Exception Safety

Use RAII and smart pointers.

Avoid:

new
delete

Prefer:

vector
string
unique_ptr

Guarantees:

* No leaks
* Safe cleanup

⸻

37. SOLID Principles

S — Single Responsibility Principle

One class = one responsibility.

⸻

O — Open Closed Principle

Open for extension.

Closed for modification.

⸻

L — Liskov Substitution Principle

Derived objects should replace base objects safely.

⸻

I — Interface Segregation Principle

Prefer many small interfaces.

⸻

D — Dependency Inversion Principle

Depend on abstractions, not implementations.

⸻

38. Static vs Dynamic Binding

Static Binding

Resolved at compile time.

Examples:

* Function overloading
* Operator overloading

Fast.

⸻

Dynamic Binding

Resolved at runtime.

Uses virtual functions.

Flexible but slightly slower.

⸻

39. vtable and vptr

Every polymorphic class contains:

vtable

Table of virtual functions.

vptr

Hidden pointer inside object.

During runtime:

Shape*
 ↓
vptr
 ↓
vtable
 ↓
Circle::area()

Enables runtime polymorphism.

⸻

40. Summary

Four Pillars

✔ Encapsulation

✔ Abstraction

✔ Inheritance

✔ Polymorphism

⸻

Relationships

✔ IS-A → Inheritance

✔ HAS-A → Composition/Aggregation

✔ USES-A → Dependency

⸻

Polymorphism

Compile Time

* Function Overloading
* Constructor Overloading
* Operator Overloading

Runtime

* Virtual Functions
* Abstract Classes
* Dynamic Binding

⸻

Memory Management

* RAII
* Rule of Three
* Rule of Five
* Rule of Zero
* Smart Pointers

⸻

Modern C++ Practices

✔ Initialization Lists

✔ const Correctness

✔ Virtual Destructors

✔ Smart Pointers

✔ Templates

✔ SOLID Principles

✔ Exception Safety

✔ Avoid raw new/delete

⸻

“OOP is not merely about classes and inheritance; it is about modeling real-world relationships while ensuring maintainability, extensibility, and safe resource management.”