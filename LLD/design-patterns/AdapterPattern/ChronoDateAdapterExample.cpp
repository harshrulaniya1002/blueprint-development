#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
// App expects this interface
class IDateProvider {
public:
    virtual std::string getCurrentDate() = 0;
};
// Adapter for std::chrono
class ChronoDateAdapter : public IDateProvider {
public:
    std::string getCurrentDate() override {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        return std::string(std::ctime(&t));
    }
};
int main() {
    ChronoDateAdapter adapter;
    std::cout << "Current date/time: " << adapter.getCurrentDate();
    return 0;
}
