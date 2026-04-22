#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Product interface
class Notification {
public:
    virtual void notify(const string& message) = 0;
    virtual ~Notification() {}
};

class EmailNotification : public Notification {
public:
    void notify(const string& message) override {
        cout << "Email: " << message << endl;
    }
};

class SMSNotification : public Notification {
public:
    void notify(const string& message) override {
        cout << "SMS: " << message << endl;
    }
};

class PushNotification : public Notification {
public:
    void notify(const string& message) override {
        cout << "Push: " << message << endl;
    }
};

// Simple Factory
class NotificationFactory {
public:
    static unique_ptr<Notification> createNotification(const string& type) {
        if (type == "email") return make_unique<EmailNotification>();
        if (type == "sms") return make_unique<SMSNotification>();
        if (type == "push") return make_unique<PushNotification>();
        return nullptr;
    }
};

int main() {
    auto n1 = NotificationFactory::createNotification("email");
    n1->notify("Welcome to our service!");
    auto n2 = NotificationFactory::createNotification("sms");
    n2->notify("Your OTP is 123456");
    auto n3 = NotificationFactory::createNotification("push");
    n3->notify("You have a new message");
    return 0;
}
