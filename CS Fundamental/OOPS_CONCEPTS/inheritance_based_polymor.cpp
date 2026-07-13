#include <iostream>

// Base Class
class Shape {
public:
    Shape() {}
    
    // Base implementation (will be overridden)
    double area() const {
        return 0.0;
    }
    
    virtual ~Shape() {} // Good practice
};

// Derived Class 1
class Square : public Shape {
private:
    double side;
public:
    Square(double s) : side(s) {}
    
    // Override area()
    double area() const {
        return side * side;
    }
};

// Derived Class 2
class Triangle : public Shape {
private:
    double base;
    double height;
public:
    Triangle(double b, double h) : base(b), height(h) {}
    
    // Override area()
    double area() const {
        return (base * height) / 2.0;
    }
};

int main() {
    Square s(5.0);
    Triangle t(4.0, 6.0);
    
    // Demonstrate direct calls
    std::cout << "Square Area (5x5): " << s.area() << std::endl; // 25.0
    std::cout << "Triangle Area (4x6): " << t.area() << std::endl; // 12.0
    
    // Base class behavior
    Shape generic;
    std::cout << "Generic Shape Area: " << generic.area() << std::endl; // 0.0

    return 0;
}