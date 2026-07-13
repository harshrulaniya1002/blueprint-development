1) const keyword is used after the member functions, so that data members can't be modify inside the function

1.1) int func() const {}
1.2) int func(const int x) {}



2) we generally dont put attributes public, because any one can access and modify them.
This exercise focuses on Encapsulation combined with controlled retrieval. By making the data private and providing the get_details() method, the class controls how its internal data is presented to the outside world.

3)This demonstrates Constructor Overloading, a form of Static Polymorphism.
The compiler selects the correct constructor based on the number and type of arguments provided during object creation.
A default constructor is essential for creating objects when no initial values are specified (e.g., in arrays or STL containers).

4) The Destructor (~Classname) is a special member function that is automatically called when an object’s lifetime ends, which occurs when it goes out of scope (as seen in scope_test() and main()):

Its primary purpose is cleanup. It ensures that any dynamically allocated memory or external resources (files, network connections, etc.) are properly cleaned up when the object is no longer needed.
This is fundamental to C++’s powerful RAII (Resource Acquisition Is Initialization) paradigm, which prevents memory leaks and resource exhaustion by tying resource lifetime to object lifetime.

5)The Copy Constructor (Point(const Point& other)) is automatically invoked whenever a new object is initialized from an existing object of the same type (e.g., Point p2 = p1; or passing an object by value to a function):

It ensures the new object is an independent copy.
For simple classes like Point, the default, compiler-generated copy constructor performs a member-wise copy (shallow copy), which is often sufficient.
However, when a class manages pointers or dynamic memory, a custom copy constructor is vital to perform a deep copy. This prevents two objects from sharing and modifying the same underlying memory block, avoiding double-free errors upon destruction.

6)This exercise highlights that constructors are not just for initialization but are also the ideal place for object validation.

By placing validation logic inside the constructor, we ensure that an object is never created in an invalid or inconsistent state. This principle contributes significantly to robust Encapsulation and object integrity.


7)The Initialization List is the preferred and often necessary way to initialize members in C++. Unlike assignments inside the constructor body, initialization lists directly initialize the members when the object is created. This is crucial for:

Efficiency: It avoids a temporary default construction followed by an assignment.
Necessity: It is required for initializing const members (like name above) and reference members, as these cannot be assigned a value after they are created.
Base Classes: It’s also used to call constructors of base classes in inheritance.

8)This exercise demonstrates how Data Hiding is used to enforce data integrity and create immutable properties.

Immutability: By making the year private and providing no public setter, the class guarantees that the year of a Car object, once set by the constructor, cannot be changed during the object’s lifetime.
Controlled Mutability: In contrast, the make property is still mutable because a set_make() method was provided.
This fine-grained control over which attributes are modifiable is a core benefit of strong Encapsulation.

9)This demonstrates the use of Static Members and Controlled Mutability on a class-level variable.

Static Member: next_serial_number belongs to the class itself, not to any individual object. All instances of SerialGenerator share the same counter.
Data Control: By making the counter private, the only way to read and advance the number is through the get_serial() method. This ensures that every call to get_serial() gets a unique number and maintains the correct sequence.
The peek_next_serial() static method shows how to access a static member without needing an object instance (g1 or g2).

non static memeber cannot be used in static member function?


10)This introduces Inheritance and Method Overriding. Inheritance is the mechanism that allows one class (Dog) to acquire the properties and methods of another class (Animal).

Single Inheritance: Dog inherits from one base class, Animal.
Code Reusability: The Dog class automatically gets the sleep() method without needing to redefine it, reducing code duplication.
Method Overriding: By defining a new eat() method in Dog with the exact same signature as the one in Animal, the derived class replaces the base class’s implementation when called on a Dog object.

11)Multilevel Inheritance creates a deep hierarchy where a class inherits from a class that, in turn, inherits from another class.
Hierarchy: SportsCar is a Car, and a Car is a Vehicle. SportsCar inherits the methods and properties from both its ancestors.
Constructor Chaining: Crucially, the constructor of SportsCar must first call the constructor of Car, which then calls the constructor of Vehicle, ensuring that all inherited private data is properly initialized.

12) The protected access specifier offers a balance between private and public access.

Internal Access: protected members are directly accessible by methods of the class itself and by methods of any classes derived from it (Student and Teacher).
External Restriction: Like private members, protected members are inaccessible from outside the class hierarchy (e.g., in the main function).
This is useful for defining attributes or helper methods that should be shared among an inheritance family but hidden from the general program logic

13) Constructor Chaining is the process by which a derived class constructor ensures that all its base class constructors are executed before its own body runs.

Initialization List is Key: By placing Car(y, doors) in SportsCar‘s initialization list, we explicitly pass arguments up the hierarchy. This ensures the correct, parameterized constructors are called at every level.

Execution Order: When SportsCar is instantiated:

Vehicle constructor runs.
Car constructor runs.
SportsCar constructor runs.
This guarantees that all inherited members are properly initialized before the derived class uses them

14)This exercise establishes the structure for Inheritance-based Polymorphism, even though the area() method is not yet declared as virtual.

Common Interface: All derived classes share the same public method signature (area()), creating a common interface for area calculation.
Overriding: Each derived class provides its specific implementation of area() based on its unique geometric properties.
This is the essential setup for Static Polymorphism (function overloading/hiding). To achieve Runtime Polymorphism (calling the correct derived method via a base class pointer/reference), the area() method in the base class must be declared virtual, which is the focus of the next set of exercises.

15)By declaring area() as a pure virtual function, we create an Abstract Base Class (Shape).

Abstraction: The Shape class now acts as an interface or a blueprint. It declares what derived classes must do (area()) but provides no implementation details.
Enforcement: Any derived class that intends to be instantiated (a concrete class) must provide an implementation for all inherited pure virtual functions.
Inability to Instantiate: Because the Shape class is incomplete (it has a function without a definition), the compiler prevents direct creation of Shape objects.

16)This is the canonical example of Runtime Polymorphism, achieved through virtual functions and pointers (or references) to the base class.

Mechanism: The Shape pointer (or std::unique_ptr<Shape>) doesn’t know the exact object type it points to until runtime. The compiler uses the v-table (virtual table) lookup to correctly dispatch the call to the appropriate derived function (Square::area() or Triangle::area()).
Dynamic Binding: The decision of which function to run is made at runtime, known as dynamic binding. This allows us to treat a collection of different object types uniformly via their common base class interface.

17)The Virtual Destructor is essential in an inheritance hierarchy involving polymorphism and dynamic memory.

The Problem (Non-Virtual): When delete ptr; is executed on a base class pointer (Base*) pointing to a derived object (Derived), the compiler performs static binding for the destructor unless it’s virtual. It only calls Base::~Base(), skipping Derived::~Derived().
The Result: The resource (new int) allocated in the derived class is never freed, leading to a memory leak.
The Fix: Declaring the base class destructor as virtual enables dynamic binding for the destructor call, ensuring that the most derived destructor (Derived::~Derived()) is called first, which then correctly calls the base destructors up the chain, preventing the leak.

18)Function Overloading (also known as Static Polymorphism or Compile-time Polymorphism) allows multiple functions within the same scope to share the same name.

Resolution: The C++ compiler resolves which function to call based on the function’s signature (the name and the argument types/number) at compile time.
Clarity: It improves code readability by allowing a single, logical name (print_info) to be used for methods that perform the same general task but handle different types of input data.

19)Operator Overloading allows standard C++ operators (like +, -, *, etc.) to be redefined for use with user-defined classes. This enhances code intuition and readability.

Syntax: When overloaded as a member function, the left-hand operand is implicitly the object (p1) and the right-hand operand is passed as the argument (p2).
Return Value: The operator returns a new Point object, mimicking the mathematical behavior of addition where the operands are unchanged (maintained by the const on other and the function itself).

20) Overloading the Subscript Operator ([]) allows a class object to be accessed using array notation, making user-defined containers behave like built-in arrays.

Reference Return: Returning an int& allows the operator to be used on the left side of an assignment (e.g., arr[2] = 99;), which modifies the original element.
Data Integrity: The implementation adds crucial boundary checking. This improves safety by catching common programming errors (accessing memory outside the array limits) and is a key feature that distinguishes a safe container class from a raw C-style array.

21) Composition models a strong “Has-A” relationship where the component object (Engine) is an integral part of the container object (Car) and generally cannot exist independently of it.

Dependence: The Engine object is created and destroyed along with the Car object, demonstrating the tight lifecycle binding.
Ownership: The Car object has sole responsibility for the lifetime of its Engine component, making it the owner. This is typically implemented by including the component as a regular value member.
Delegation: The Car::drive() method delegates the core starting behavior to its internal Engine component, simplifying the car’s implementation.

22) Aggregation also models a “Has-A” relationship, but it’s weaker than composition. The key difference is the independent lifecycle of the parts (Employee).

Independent Lifecycle: Employee objects can exist and be destroyed regardless of whether the Department object exists.
Implementation: This is typically implemented by storing pointers or references to the components. The container (Department) only knows about the parts; it does not own their memory.
Flexibility: Aggregation is used when components are shared among multiple containers or when components’ lifetimes are managed externally (e.g., via a garbage collector or specific memory allocation logic).

23)This exercise clearly illustrates the memory management implications of Aggregation in the context of a library system.

External Ownership: The Book objects are created using new in main, meaning they exist on the heap, and their memory is controlled by the main function’s scope.
Container Role: The Library only stores the addresses (pointers) to these external objects. It acts as a logical container but does not assume memory responsibility.
Key Difference: If this were Composition, the Library would create the books itself (e.g., using std::vector<Book>) and its destructor would automatically destroy them.

24) This models a One-to-Many relationship using Composition, which is often the best choice for modeling groups where the existence of the members is entirely dependent on the group.

Strong Ownership: By using std::vector<Player>, the Team object directly owns the Player objects. The Player objects are created/copied when the Team object is constructed and are automatically destroyed when the Team object is destroyed.
Memory Management: The use of std::vector automates memory management, ensuring no memory leaks and demonstrating a clean composition lifecycle, as the Team is solely responsible for its players.

25)A Static Member Variable (or class variable) is a single, shared instance of a variable that exists regardless of how many objects of the class are created.

Shared State: total_accounts is shared by all BankAccount objects and is stored separately from the individual object data.
Lifecycle Tracking: By incrementing in the constructor and decrementing in the destructor, the static variable accurately tracks the live count of objects currently instantiated, providing crucial class-level statistics.
Initialization: Static non-const members must be defined and initialized outside the class body, as shown by int BankAccount::total_accounts = 0;

26)A Friend Function is a function defined outside a class’s scope but given special permission to access the private and protected members of that class.

Breaking Encapsulation (Controlled): Friendship is a way to intentionally relax strict encapsulation when necessary, often for overloaded operators (like operator<< in Ex. 28) or utility functions closely tied to the class’s data.
Non-Member Status: calculate_volume is not a member of the Box class. It is a regular external function, but the friend declaration inside Box grants the necessary access rights.
Use Case: Here, it allows a logically related but non-member utility function to perform its task efficiently by avoiding public getters for simple data access.

27)A Friend Class grants all methods within the accessor class the ability to access the private and protected members of the class that declared the friendship.

Broad Access: Unlike friend functions, which grant access only to a single function, friend classes grant blanket access to the entire class of methods.
Use Case: This pattern is useful for unit testing or when implementing an internal helper class (like an iterator or a specialized manager) that must be tightly coupled with the private implementation of another class.