#include <iostream>
#include <vector>
#include <memory> // For std::unique_ptr (better resource management)

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
    // Use smart pointers (unique_ptr) for automatic cleanup and safety
    std::vector<std::unique_ptr<Shape>> shapes;
    
    // Store derived objects via base class pointers
    shapes.push_back(std::make_unique<Square>(8.0));
    shapes.push_back(std::make_unique<Triangle>(10.0, 5.0));
    shapes.push_back(std::make_unique<Square>(3.0));
    
    std::cout << "--- Calculating Areas (Runtime Polymorphism) ---" << std::endl;
    
    for (const auto& shape_ptr : shapes) {
        // The call to shape_ptr->area() executes the correct 
        // derived method (Square::area or Triangle::area) at RUNTIME.
        std::cout << "Area: " << shape_ptr->area() << std::endl; 
    }
    
    // Smart pointers automatically delete the objects when 'shapes' goes out of scope.

    return 0;
}