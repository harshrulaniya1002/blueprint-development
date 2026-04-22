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
        cout << "Preparing Basic Burger with lettuce, tomato, and cheese\n";
    }
};

class StandardBurger: public Burger {
public:
    void prepare() override {
        cout << "Preparing Standard Burger with lettuce, tomato, cheese, and pickles\n";
    }
};

class PremiumBurger: public Burger {
public:
    void prepare() override {
        cout << "Preparing Premium Burger with lettuce, tomato, cheese, pickles, and bacon\n";
    }
};

class BasicWheatBurger: public Burger {
public:
    void prepare() override {
        cout << "Preparing Basic Wheat Burger with lettuce, tomato, and cheese\n";
    }
};

class StandardWheatBurger: public Burger {
public:
    void prepare() override {
        cout << "Preparing Standard Wheat Burger with lettuce, tomato, and cheese\n";
    }
};

class PremiumWheatBurger: public Burger {
public:
    void prepare() override {
        cout << "Preparing Premium Wheat Burger with lettuce, tomato, and cheese\n";
    }
};

//Factory and its concretions
class BurgerFactory {
    public:
    virtual Burger* createBurger(string& type) = 0;
};

class SinghBurger : public BurgerFactory {
    public:
    Burger* createBurger(string& type) override {
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

class KingBurger : public BurgerFactory {
    public:
    Burger* createBurger(string& type) override {
        if(type == "Basic") {
            return new BasicWheatBurger();
        } else if(type == "Standard") {
            return new StandardWheatBurger();
        } else if(type == "Premium") {
            return new PremiumWheatBurger();
        } else {
            cout << "Unknown burger type: " << type << "\n";
            return nullptr;
        }
    }
};

int main() {
    BurgerFactory* factory;

    string type;
    cout << "Enter the type of burger (Basic/Standard/Premium): ";
    cin >> type;

    string brand;
    cout << "Enter the brand (Singh/King): ";
    cin >> brand;

    if (brand == "Singh") {
        factory = new SinghBurger();
    } else if (brand == "King") {
        factory = new KingBurger();
    } else {
        cout << "Unknown brand: " << brand << "\n";
        return 1;
    }

    Burger* burger = factory->createBurger(type);
    if(burger) {
        burger->prepare();
        delete burger;
    }

    delete factory;
    return 0;
}
