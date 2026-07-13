#include <iostream>
#include <string>
#include <vector>

// Part Class (Independent existence)
class Book {
private:
    std::string title;
public:
    Book(std::string t) : title(t) {}
    
    // CORRECTION: Return type changed from void to std::string
    std::string get_details() const { 
        return "  - Book Title: " + title;
    }
};

// Whole Class (using Aggregation)
class Library {
private:
    std::string name;
    // Aggregation: Stores pointers to books owned by the main program/user
    std::vector<Book*> catalog; 

public:
    Library(std::string n) : name(n) {}

    void add_book(Book* book) {
        catalog.push_back(book);
        // Works now because get_details() returns std::string
        std::cout << "[Library] Added: " << book->get_details() << std::endl;
    }

    void list_books() const {
        std::cout << "\n--- " << name << " Catalog ---" << std::endl;
        if (catalog.empty()) {
            std::cout << "Catalog is empty." << std::endl;
        } else {
            for (const auto* book : catalog) {
                // Works now because get_details() returns std::string
                std::cout << book->get_details() << std::endl; 
            }
        }
    }
    
    // Destructor: No memory cleanup for the Book pointers
    ~Library() {
        std::cout << "\nLibrary closed. (Books remain in memory until explicitly deleted.)" << std::endl;
    }
};

int main() {
    // Books are owned by main scope
    Book* b1 = new Book("C++ Primer");
    Book* b2 = new Book("Effective Modern C++");

    Library central("Central Library");
    central.add_book(b1);
    central.add_book(b2);
    
    central.list_books();
    
    // Explicit external cleanup is necessary in aggregation
    delete b1;
    delete b2;
    
    return 0;
}