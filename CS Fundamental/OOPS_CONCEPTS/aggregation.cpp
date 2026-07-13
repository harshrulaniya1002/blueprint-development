#include <iostream>
#include <string>
#include <vector>

// Part Class (Independent existence)
class Employee {
private:
    std::string name;
public:
    Employee(std::string n) : name(n) {
        std::cout << "Employee " << name << " hired." << std::endl;
    }
    void work() const {
        std::cout << name << " is performing tasks." << std::endl;
    }
    ~Employee() {
        std::cout << "Employee " << name << " retired/left." << std::endl;
    }
};

// Whole Class (using Aggregation)
class Department {
private:
    std::string dept_name;
    // Aggregation: Stores pointers/references to objects owned elsewhere.
    std::vector<Employee*> staff; 
    
public:
    Department(std::string name) : dept_name(name) {}

    // Adds an Employee pointer owned outside the department
    void add_employee(Employee* emp) {
        staff.push_back(emp);
        std::cout << "[Dept] Employee added to " << dept_name << "." << std::endl;
    }

    void list_staff() const {
        std::cout << "\n--- " << dept_name << " Staff ---" << std::endl;
        for (const auto* emp : staff) {
            emp->work();
        }
    }
    
    // IMPORTANT: No 'delete' for Employee pointers in the Department destructor.
    ~Department() {
        std::cout << "\nDepartment " << dept_name << " shut down." << std::endl;
    }
};

int main() {
    // 1. Employees (parts) are created independently (on the heap)
    Employee* alice = new Employee("Alice");
    Employee* bob = new Employee("Bob");

    // 2. Department (whole) is created
    Department hr_dept("HR");
    
    // 3. Department uses the employees
    hr_dept.add_employee(alice);
    hr_dept.add_employee(bob);
    hr_dept.list_staff();

    // 4. Department goes out of scope and is destroyed, 
    // but Alice and Bob still exist until manually deleted.
    
    // Cleanup must be done externally (memory management not tied to Department)
    delete alice; 
    delete bob;
    
    return 0;
}