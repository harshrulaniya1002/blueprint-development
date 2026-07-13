#include <iostream>

// Abstract Base Class (ABC)
class Shape {
public:
    // Pure virtual function: makes Shape an ABC
    virtual double area() const = 0;
    
    // Virtual destructor is good practice for base classes
    virtual ~Shape() {} 
};

// Derived Class 1
class Square : public Shape {
private:
    double side;
public:
    Square(double s) : side(s) {}
    
    // Must implement area() to be concrete
    double area() const override {
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
    
    // Must implement area() to be concrete
    double area() const override {
        return (base * height) / 2.0;
    }
};

int main() {
    // Shape s; // Compiler Error: cannot instantiate abstract class 'Shape'

    // Must use derived classes
    Square s(5.0);
    Triangle t(4.0, 6.0);
    
    std::cout << "Square Area (5x5): " << s.area() << std::endl;
    std::cout << "Triangle Area (4x6): " << t.area() << std::endl;

    return 0;
}