#include <iostream>
#include <fstream>
#include <memory>
#include <string>
using namespace std;

// Strategy interface
class LoggingStrategy {
public:
    virtual void log(const string& message) = 0;
    virtual ~LoggingStrategy() {}
};

class ConsoleLogger : public LoggingStrategy {
public:
    void log(const string& message) override {
        cout << "[Console] " << message << endl;
    }
};

class FileLogger : public LoggingStrategy {
    string filename;
public:
    FileLogger(const string& fname) : filename(fname) {}
    void log(const string& message) override {
        ofstream ofs(filename, ios::app);
        ofs << "[File] " << message << endl;
    }
};

class NetworkLogger : public LoggingStrategy {
public:
    void log(const string& message) override {
        cout << "[Network] Sending log: " << message << endl;
        // Simulate sending log over network
    }
};

// Context
class Application {
    unique_ptr<LoggingStrategy> logger;
public:
    void setLogger(LoggingStrategy* l) {
        logger.reset(l);
    }
    void process(const string& task) {
        if (logger)
            logger->log("Processing task: " + task);
        else
            cout << "No logger set!\n";
    }
};

int main() {
    Application app;
    app.setLogger(new ConsoleLogger());
    app.process("User login");

    app.setLogger(new FileLogger("app.log"));
    app.process("Data export");

    app.setLogger(new NetworkLogger());
    app.process("Remote sync");
    return 0;
}
