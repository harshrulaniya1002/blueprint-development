#include <iostream>
#include <vector>
#include <string>
using namespace std;

class NewsSubscriber {
public:
    virtual void update(const string& headline) = 0;
};

class NewsAgency {
    vector<NewsSubscriber*> subscribers;
public:
    void addSubscriber(NewsSubscriber* sub) { subscribers.push_back(sub); }
    void publish(const string& headline) {
        for (auto* sub : subscribers) sub->update(headline);
    }
};

class EmailSubscriber : public NewsSubscriber {
public:
    void update(const string& headline) override {
        cout << "[Email] Breaking News: " << headline << endl;
    }
};

class SmsSubscriber : public NewsSubscriber {
public:
    void update(const string& headline) override {
        cout << "[SMS] Breaking News: " << headline << endl;
    }
};

int main() {
    NewsAgency agency;
    EmailSubscriber email;
    SmsSubscriber sms;
    agency.addSubscriber(&email);
    agency.addSubscriber(&sms);
    agency.publish("New Observer Pattern Example Released!");
    agency.publish("Weather Alert: Heavy Rain Expected Tomorrow.");
    return 0;
}
