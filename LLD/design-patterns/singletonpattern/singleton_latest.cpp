#include<iostream>
using namespace std;

class DatabaseConnection {
    private:
        DatabaseConnection() {
            cout << "Database Connection Created!" << endl;
        }

    public:
    static DatabaseConnection& getInstance() {
        //thread safe in C++11 and later due to guaranteed thread-safe initialization of function-local static variables
        static DatabaseConnection instance;
        return instance;
    }    
    
    // Delete copy constructor and copy assignment operator to prevent copying of the singleton instance
    DatabaseConnection(const DatabaseConnection&) = delete; // Delete copy constructor
    DatabaseConnection& operator=(const DatabaseConnection&) = delete; // Delete copy assignment operator
};


int main()
{
    DatabaseConnection& db1 = DatabaseConnection::getInstance();
    DatabaseConnection& db2 = DatabaseConnection::getInstance();
    
    // db1 = db2; // This line will cause a compile-time error due to the deleted copy assignment operator

    if (&db1 == &db2) {
        cout << "Both db1 and db2 are the same instance." << endl;
    } else {
        cout << "db1 and db2 are different instances." << endl;
    }
    return 0;
}