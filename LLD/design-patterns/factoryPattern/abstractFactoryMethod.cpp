#include<bits/stdc++.h>
using namespace std;

//Product 1 Burger
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

// Product 2 Garlic Bread
class GarlicBread {
public:
    virtual void prepare() = 0; // pure virtual function
    virtual ~GarlicBread() {} // virtual destructor
};

class BasicGarlicBread: public GarlicBread {
public:
    void prepare() override {
        cout << "Preparing Basic Garlic Bread with butter and garlic\n";
    }
};

class StandardGarlicBread: public GarlicBread {
public:
    void prepare() override {
        cout << "Preparing Standard Garlic Bread with butter, garlic, and herbs\n";
    }
};

class PremiumGarlicBread: public GarlicBread {
public:
    void prepare() override {
        cout << "Preparing Premium Garlic Bread with butter, garlic, herbs, and cheese\n";
    }
};

class BasicWheatGarlicBread: public GarlicBread {
public:
    void prepare() override {
        cout << "Preparing Basic Wheat Garlic Bread with butter and garlic\n";
    }
};

class StandardWheatGarlicBread: public GarlicBread {
public:
    void prepare() override {
        cout << "Preparing Standard Wheat Garlic Bread with butter, garlic, and herbs\n";
    }
};

class PremiumWheatGarlicBread: public GarlicBread {
public:
    void prepare() override {
        cout << "Preparing Premium Wheat Garlic Bread with butter, garlic, herbs, and cheese\n";
    }
};

//Factory and its concretions
class MealFactory {
    public:
    virtual Burger* createBurger(string& type) = 0;
    virtual GarlicBread* createGarlicBread(string& type) = 0;
};

class SinghBurger : public MealFactory {
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

    GarlicBread* createGarlicBread(string& type) override {
        if(type == "Basic") {
            return new BasicGarlicBread();
        } else if(type == "Standard") {
            return new StandardGarlicBread();
        } else if(type == "Premium") {
            return new PremiumGarlicBread();
        } else {
            cout << "Unknown garlic bread type: " << type << "\n";
            return nullptr;
        }
    }
};

class KingBurger : public MealFactory {
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

    GarlicBread* createGarlicBread(string& type) override {
        if(type == "Basic") {
            return new BasicWheatGarlicBread();
        } else if(type == "Standard") {
            return new StandardWheatGarlicBread();
        } else if(type == "Premium") {
            return new PremiumWheatGarlicBread();
        } else {
            cout << "Unknown garlic bread type: " << type << "\n";
            return nullptr;
        }
    }
};

int main() {
    MealFactory* burgerFactory;
    MealFactory* garlicBreadFactory;

    string burgerType, garlicBreadType;
    string burgerBrand, garlicBreadBrand;

    cout << "Enter the brand for burger (Singh/King): ";
    cin >> burgerBrand;
    cout << "Enter the type of burger (Basic/Standard/Premium): ";
    cin >> burgerType;

    cout << "Enter the brand for garlic bread (Singh/King): ";
    cin >> garlicBreadBrand;
    cout << "Enter the type of garlic bread (Basic/Standard/Premium): ";
    cin >> garlicBreadType;

    // Select burger factory
    if (burgerBrand == "Singh") {
        burgerFactory = new SinghBurger();
    } else if (burgerBrand == "King") {
        burgerFactory = new KingBurger();
    } else {
        cout << "Unknown burger brand: " << burgerBrand << "\n";
        return 1;
    }

    // Select garlic bread factory
    if (garlicBreadBrand == "Singh") {
        garlicBreadFactory = new SinghBurger();
    } else if (garlicBreadBrand == "King") {
        garlicBreadFactory = new KingBurger();
    } else {
        cout << "Unknown garlic bread brand: " << garlicBreadBrand << "\n";
        delete burgerFactory;
        return 1;
    }

    Burger* burger = burgerFactory->createBurger(burgerType);
    if (burger) {
        burger->prepare();
        delete burger;
    }

    GarlicBread* garlicBread = garlicBreadFactory->createGarlicBread(garlicBreadType);
    if (garlicBread) {
        garlicBread->prepare();
        delete garlicBread;
    }

    delete burgerFactory;
    delete garlicBreadFactory;
    return 0;
}
