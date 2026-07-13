#include<iostream>
using namespace std;

class circle{
    private:
    double radius;
    const double PI = 3.14159;

    public:
    circle(double r): radius(r){
        cout<<"circle constructor initializing the radius"<<"\n";
    };
    

    //read function, not mutator
    int get_area() const {
        return PI*radius*radius;
    }

};

int main(){
  circle c(7);
  cout<<"area = "<<c.get_area()<<"\n";
}