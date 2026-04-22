#include <iostream>
#include <vector>
using namespace std;

class WeatherObserver {
public:
    virtual void update(float temp, float humidity) = 0;
};

class WeatherStation {
    vector<WeatherObserver*> observers;
    float temperature;
    float humidity;
public:
    void addObserver(WeatherObserver* obs) { observers.push_back(obs); }
    void setMeasurements(float temp, float hum) {
        temperature = temp; humidity = hum;
        for (auto* obs : observers) obs->update(temperature, humidity);
    }
};

class PhoneDisplay : public WeatherObserver {
public:
    void update(float temp, float humidity) override {
        cout << "[PhoneDisplay] Temp: " << temp << ", Humidity: " << humidity << endl;
    }
};

class WindowDisplay : public WeatherObserver {
public:
    void update(float temp, float humidity) override {
        cout << "[WindowDisplay] Temp: " << temp << ", Humidity: " << humidity << endl;
    }
};

int main() {
    WeatherStation station;
    PhoneDisplay phone;
    WindowDisplay window;
    station.addObserver(&phone);
    station.addObserver(&window);
    station.setMeasurements(30.5, 60.0);
    station.setMeasurements(28.0, 55.0);
    return 0;
}
