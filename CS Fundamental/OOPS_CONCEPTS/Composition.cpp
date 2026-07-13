#include <iostream>
#include <string>

// Component Class
class Engine {
private:
    std::string type;
public:
    Engine(std::string t) : type(t) {
        std::cout << "Engine (" << type << ") constructed." << std::endl;
    }
    
    void start() const {
        std::cout << "--> " << type << " engine starting up..." << std::endl;
    }
};

// Container Class (using Composition)
class Car {
private:
    std::string model;
    // Composition: Engine object is fully owned by Car. 
    // It cannot exist without the Car.
    Engine engine; 
    
public:
    // Car constructor initializes its own members and the Engine component.
    Car(std::string m, std::string engineType) 
        : model(m), 
          engine(engineType) // Initialize the Engine component
    {
        std::cout << "Car (" << model << ") assembled." << std::endl;
    }

    void drive() const {
        std::cout << "\nStarting " << model << " drive." << std::endl;
        engine.start(); // Car delegates the action to its component
    }
    
    // Destructor (implicitly called, but shows relationship)
    ~Car() {
        std::cout << "Car (" << model << ") destroyed." << std::endl;
        // Engine's destructor is automatically called here.
    }
};

int main() {
    Car my_car("SedanX", "V6");
    my_car.drive();
    
    std::cout << "\nEnd of main scope." << std::endl;
    return 0;
}