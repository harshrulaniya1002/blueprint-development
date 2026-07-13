// write a class player that contains atrributes for player name,age,teamName;
//  write three member functions input data, change data, display data attributes
// write a constructor that ask for input to initialize all the attributes.

#include<iostream>
using namespace std;

class Player{
    private:
    string name;
    int age;
    string teamName;


    public:
    Player(){
        //constructor
       inputData();
    }

    void changeData(){
        cout<<"Enter new player Details: "<<"\n";
        inputData();
    }

    void inputData(){
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout<<"enter the player name"<<"\n";
        getline(cin,name);

        cout<<"enter the age"<<"\n";
        cin>>age;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout<<"enter the player teamName"<<"\n";
        getline(cin,teamName);
    }

    void Display(){
        cout << "\n----- Player Details -----\n";
        cout << "Name      : " << name << '\n';
        cout << "Age       : " << age << '\n';
        cout << "Team Name : " << teamName << '\n';
    }


};

int main(){
   int choice;
   Player p;
   do {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Input Player Data\n";
        cout << "2. Display Player Data\n";
        cout << "3. Change Player Data\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                p.inputData();
                break;
            case 2:
                p.Display();
                break;    
            case 3:
                p.changeData();
                break;
            case 4:
                exit(1);
                cout<<"Exiting..."<<"\n";
                break;
            default:
                cout<<"You Entered Invalid Number"<<"\n";
        }
   } while(choice != 4);

}