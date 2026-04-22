#include <iostream>
#include <string>
using namespace std;

class DatabaseConnection {
    static DatabaseConnection* instance;
    string connectionString;
    DatabaseConnection() : connectionString("Server=localhost;DB=app;") {}
public:
    static DatabaseConnection* getInstance() {
        if (!instance)
            instance = new DatabaseConnection();
        return instance;
    }
    void connect() {
        cout << "Connecting to DB with: " << connectionString << endl;
    }
    void setConnectionString(const string& cs) {
        connectionString = cs;
    }
};
DatabaseConnection* DatabaseConnection::instance = nullptr;

int main() {
    auto* db = DatabaseConnection::getInstance();
    db->connect();
    db->setConnectionString("Server=prod;DB=main;");
    db->connect();
    return 0;
}
