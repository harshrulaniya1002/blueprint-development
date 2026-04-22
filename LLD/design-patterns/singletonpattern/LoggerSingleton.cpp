#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Logger {
    static Logger* instance;
    ofstream logFile;
    Logger() { logFile.open("app.log", ios::app); }
public:
    static Logger* getInstance() {
        if (!instance)
            instance = new Logger();
        return instance;
    }
    void log(const string& message) {
        logFile << message << endl;
        cout << "[LOG]: " << message << endl;
    }
    ~Logger() { logFile.close(); }
};
Logger* Logger::instance = nullptr;

int main() {
    Logger::getInstance()->log("Application started");
    Logger::getInstance()->log("Another log entry");
    return 0;
}
