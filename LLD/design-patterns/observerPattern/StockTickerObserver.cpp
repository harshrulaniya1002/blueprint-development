#include <iostream>
#include <vector>
#include <string>
using namespace std;

class StockObserver {
public:
    virtual void update(const string& symbol, double price) = 0;
};

class StockTicker {
    vector<StockObserver*> observers;
    string symbol;
    double price;
public:
    void addObserver(StockObserver* obs) { observers.push_back(obs); }
    void setPrice(const string& sym, double p) {
        symbol = sym; price = p;
        for (auto* obs : observers) obs->update(symbol, price);
    }
};

class MobileApp : public StockObserver {
public:
    void update(const string& symbol, double price) override {
        cout << "[MobileApp] " << symbol << " price updated: " << price << endl;
    }
};

class WebDashboard : public StockObserver {
public:
    void update(const string& symbol, double price) override {
        cout << "[WebDashboard] " << symbol << " price updated: " << price << endl;
    }
};

int main() {
    StockTicker ticker;
    MobileApp mobile;
    WebDashboard web;
    ticker.addObserver(&mobile);
    ticker.addObserver(&web);
    ticker.setPrice("AAPL", 199.5);
    ticker.setPrice("GOOG", 2850.0);
    return 0;
}
