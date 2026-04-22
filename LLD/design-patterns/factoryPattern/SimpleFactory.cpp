#include<bits/stdc++.h>
using namespace std;

class Burger{
public:
virtual void prepare() = 0; // pure virtual function
virtual ~Burger() {} //virtual destructor
};

class BasicBurger: public Burger {
public:
    void prepare() override {
        cout << "Preparing Basic Burger\n";
    }
};

class StandardBurger: public Burger {
public:
    void prepare() override {
        cout << "Preparing Standard Burger\n";
    }
};

class PremiumBurger: public Burger {
public:
    void prepare() override {
        cout << "Preparing Premium Burger\n";
    }
};

class BurgerFactory {
public:
    Burger * CreateBurger(string& type){
        if(type == "Basic") {
            return new BasicBurger();
        } else if(type == "Standard") {
            return new StandardBurger();
        } else if(type == "Premium") {
            return new PremiumBurger();
        } else {
            cout << "Unknown burger type: " << type << "\n";
            return nullptr;
        }
    }
};

int main() {
    BurgerFactory factory;
    string type;

    cout << "Enter the type of burger (Basic/Standard/Premium): ";
    cin >> type;

    Burger* burger = factory.CreateBurger(type);
    if(burger) {
        burger->prepare();
        delete burger;
    }

    return 0;
}
