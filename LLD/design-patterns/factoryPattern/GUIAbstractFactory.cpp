#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Abstract Products
class Button {
public:
    virtual void render() = 0;
    virtual ~Button() {}
};

class Menu {
public:
    virtual void render() = 0;
    virtual ~Menu() {}
};

// Concrete Products for Windows
class WindowsButton : public Button {
public:
    void render() override {
        cout << "Rendering Windows Button" << endl;
    }
};

class WindowsMenu : public Menu {
public:
    void render() override {
        cout << "Rendering Windows Menu" << endl;
    }
};

// Concrete Products for MacOS
class MacButton : public Button {
public:
    void render() override {
        cout << "Rendering MacOS Button" << endl;
    }
};

class MacMenu : public Menu {
public:
    void render() override {
        cout << "Rendering MacOS Menu" << endl;
    }
};

// Abstract Factory
class GUIFactory {
public:
    virtual unique_ptr<Button> createButton() = 0;
    virtual unique_ptr<Menu> createMenu() = 0;
    virtual ~GUIFactory() {}
};

// Concrete Factories
class WindowsFactory : public GUIFactory {
public:
    unique_ptr<Button> createButton() override {
        return make_unique<WindowsButton>();
    }
    unique_ptr<Menu> createMenu() override {
        return make_unique<WindowsMenu>();
    }
};

class MacFactory : public GUIFactory {
public:
    unique_ptr<Button> createButton() override {
        return make_unique<MacButton>();
    }
    unique_ptr<Menu> createMenu() override {
        return make_unique<MacMenu>();
    }
};

int main() {
    unique_ptr<GUIFactory> factory;
    string os = "windows";
    if (os == "windows")
        factory = make_unique<WindowsFactory>();
    else
        factory = make_unique<MacFactory>();
    auto btn = factory->createButton();
    auto menu = factory->createMenu();
    btn->render();
    menu->render();
    return 0;
}
