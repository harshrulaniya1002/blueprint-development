#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Strategy interface
class PaymentStrategy {
public:
    virtual void pay(double amount) = 0;
    virtual ~PaymentStrategy() {}
};

class CreditCardPayment : public PaymentStrategy {
    string cardNumber;
public:
    CreditCardPayment(const string& number) : cardNumber(number) {}
    void pay(double amount) override {
        cout << "Paid $" << amount << " using Credit Card (" << cardNumber << ")\n";
    }
};

class PayPalPayment : public PaymentStrategy {
    string email;
public:
    PayPalPayment(const string& email) : email(email) {}
    void pay(double amount) override {
        cout << "Paid $" << amount << " using PayPal (" << email << ")\n";
    }
};

class CryptoPayment : public PaymentStrategy {
    string walletId;
public:
    CryptoPayment(const string& walletId) : walletId(walletId) {}
    void pay(double amount) override {
        cout << "Paid $" << amount << " using Crypto Wallet (" << walletId << ")\n";
    }
};

// Context
class ShoppingCart {
    unique_ptr<PaymentStrategy> paymentStrategy;
public:
    void setPaymentStrategy(PaymentStrategy* strategy) {
        paymentStrategy.reset(strategy);
    }
    void checkout(double amount) {
        if (paymentStrategy)
            paymentStrategy->pay(amount);
        else
            cout << "No payment strategy selected!\n";
    }
};

int main() {
    ShoppingCart cart;
    cart.setPaymentStrategy(new CreditCardPayment("1234-5678-9012-3456"));
    cart.checkout(250.0);

    cart.setPaymentStrategy(new PayPalPayment("user@example.com"));
    cart.checkout(99.99);

    cart.setPaymentStrategy(new CryptoPayment("0xABCDEF123456"));
    cart.checkout(500.5);
    return 0;
}
