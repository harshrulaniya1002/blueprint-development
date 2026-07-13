#include <iostream>
using namespace std;

class Rectangle {
private:
    int length;
    int width;
public:
    // Constructor

    // static polymorphism 
    Rectangle(int length, int w) : length(length), width(w) {}
    Rectangle() : length(1), width(1) {}


    //destructor
    ~Rectangle(){
        cout<<"destructor called"<<"\n";
    }

    // Method to calculate and return the area
    int calculate_area() const {
        return length * width;
    }

    int getLength() const {
        return length;
    }

    int getWidth() const {
        return width;
    }

    // Method to calculate and return the perimeter
    int calculate_perimeter() const {
        return 2 * (length + width);
    }
};

int main() {
    Rectangle rect(10, 5); // Create a rectangle object
    Rectangle rect2;
    
    int area = rect.calculate_area();
    int area2 = rect2.calculate_area();
    int perimeter = rect.calculate_perimeter();
    int perimeter2 = rect2.calculate_perimeter();
    
    cout << "Rectangle Dimensions: " << rect.getLength() << "x" << rect.getWidth() << endl;
    cout << "Area: " << area << endl;
    cout << "Perimeter: " << perimeter << endl;

    cout << "Rectangle Dimensions: " << rect2.getLength() << "x" << rect2.getWidth() << endl;
    cout << "Area: " << area2 << endl;
    cout << "Perimeter: " << perimeter2 <<endl;
    return 0;
}