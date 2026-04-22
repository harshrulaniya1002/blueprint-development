#include <iostream>
#include <string>
// Old logger interface
class OldLogger {
public:
    void logMessage(const std::string& msg) { std::cout << "[OLD] " << msg << std::endl; }
};
// New logger interface expected by the app
class ILogger {
public:
    virtual void log(const std::string& msg) = 0;
};
// Adapter
class LoggerAdapter : public ILogger {
    OldLogger* oldLogger;
public:
    LoggerAdapter(OldLogger* logger) : oldLogger(logger) {}
    void log(const std::string& msg) override { oldLogger->logMessage(msg); }
};
int main() {
    OldLogger oldLogger;
    LoggerAdapter adapter(&oldLogger);
    adapter.log("Adapter pattern bridges old and new!");
    return 0;
}
