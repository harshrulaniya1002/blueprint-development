#include <iostream>
// Third-party PayPal API
class PayPalAPI {
public:
    void makePayment(double amount) { std::cout << "Paid " << amount << " via PayPal\n"; }
};
// App expects this interface
class IPayment {
public:
    virtual void pay(double amount) = 0;
};
// Adapter
class PayPalAdapter : public IPayment {
    PayPalAPI* api;
public:
    PayPalAdapter(PayPalAPI* p) : api(p) {}
    void pay(double amount) override { api->makePayment(amount); }
};
int main() {
    PayPalAPI paypal;
    PayPalAdapter adapter(&paypal);
    adapter.pay(99.99);
    return 0;
}
