#include<iostream>
using namespace std;

class SerialGenerator{
   private:
   static int nextSerialCounter;
   int age;
   
   public:
   SerialGenerator(int age): age(age){};

   int get_serial(){
    return nextSerialCounter++;
   }

   static int getValue(){
    return nextSerialCounter;
   }
};

int SerialGenerator:: nextSerialCounter = 1000;

int main(){ 
    //without need of object, we can use static method on static data member;
    cout<<SerialGenerator::getValue()<<"\n";

    SerialGenerator sg(23);
    cout<<sg.get_serial()<<"\n";
    cout<<sg.get_serial()<<"\n";

    SerialGenerator sg1(24);
    cout<<sg1.get_serial()<<"\n";
    cout<<sg1.get_serial()<<"\n";

    cout<<SerialGenerator::getValue()<<"\n";
}