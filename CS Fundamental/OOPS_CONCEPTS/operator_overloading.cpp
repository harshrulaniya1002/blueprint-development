#include <iostream>

class Point {
private:
    int x;
    int y;

public:
    Point(int px = 0, int py = 0) : x(px), y(py) {}

    // Overload the binary + operator as a member function
    Point operator+(const Point& other) const {
        // Create a new Point object with the summed coordinates
        return Point(this->x + other.x, this->y + other.y);
    }

    void display() const {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
};

int main() {
    Point p1(10, 5);
    Point p2(3, 7);
    
    // Uses the overloaded operator: p1.operator+(p2)
    Point p3 = p1 + p2; 
    
    std::cout << "P1: "; p1.display();
    std::cout << "P2: "; p2.display();
    std::cout << "P3 (P1 + P2): "; p3.display(); // Expected: (13, 12)

    return 0;
}