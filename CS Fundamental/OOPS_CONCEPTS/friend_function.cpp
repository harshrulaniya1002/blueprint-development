#include <iostream>

class Box {
private:
    int length;
    int width;

public:
    Box(int l, int w) : length(l), width(w) {}

    // Declaration of the non-member friend function
    friend int calculate_volume(const Box& b, int height); 
};

// Definition of the non-member function (does not use Box:: scope)
int calculate_volume(const Box& b, int height) {
    // The friend function can directly access private members 'length' and 'width'
    return b.length * b.width * height;
}

int main() {
    Box my_box(10, 5); // Length=10, Width=5
    int h = 2;

    // External function call, accessing private data
    int volume = calculate_volume(my_box, h); 
    
    std::cout << "Box dimensions: 10x5x2" << std::endl;
    std::cout << "Calculated Volume (via Friend Function): " << volume << std::endl; // 100

    return 0;
}