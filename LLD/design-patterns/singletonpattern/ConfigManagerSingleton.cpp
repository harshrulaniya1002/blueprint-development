#include <iostream>
#include <string>
#include <map>
using namespace std;

class ConfigManager {
    static ConfigManager* instance;
    map<string, string> config;
    ConfigManager() {
        config["host"] = "localhost";
        config["port"] = "8080";
    }
public:
    static ConfigManager* getInstance() {
        if (!instance)
            instance = new ConfigManager();
        return instance;
    }
    string get(const string& key) {
        return config[key];
    }
    void set(const string& key, const string& value) {
        config[key] = value;
    }
};
ConfigManager* ConfigManager::instance = nullptr;

int main() {
    auto* cfg = ConfigManager::getInstance();
    cout << "Host: " << cfg->get("host") << endl;
    cfg->set("host", "192.168.1.1");
    cout << "Host updated: " << cfg->get("host") << endl;
    return 0;
}
