#include<iostream>
using namespace std;

class Point{
   int x;
   int y;


   public:
   Point(int x, int y) : x(x), y(y){};
   //copy constructor take its value by reference
   Point(Point &other) : x(other.x), y(other.y){
    cout<<"Copy Constructor called"<<"\n";
   };

   void move(){
    x += 10;
    y += 20;
   }

   void show_cordi(){
    cout<<"x = "<<x<<"\n";
    cout<<"y = "<<y<<"\n";
   }
};

int main(){
  Point p(1,2);
  cout<<"P cordinates are"<<"\n";
  p.show_cordi();

  Point q = p;
  cout<<"Q cordinates are"<<"\n";
  q.show_cordi();

  q.move();
  cout<<"Q cordinates after move"<<"\n";
  q.show_cordi();

  cout<<"P cordinates after move remain same"<<"\n";
  p.show_cordi();

}