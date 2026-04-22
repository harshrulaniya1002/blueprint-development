#include <bits/stdc++.h>
using namespace std;

// strategy interface for Walk
class WalkableRobot
{
public:
   virtual void walk() = 0;
   virtual ~WalkableRobot() {}
};

class NormalWalk : public WalkableRobot
{
public:
   void walk() override
   {
      cout << "Walking Normally...." << "\n";
   }
};

class NoWalk : public WalkableRobot
{
public:
   void walk() override
   {
      cout << "Cannot Walk...." << "\n";
   }
};



// Strategy interface for talk
class TalkableRobot
{
public:
   virtual void talk() = 0;
   virtual ~TalkableRobot() {}
};

class NormalTalk : public TalkableRobot
{
public:
   void talk() override
   {
      cout << "Talking Normally...." << "\n";
   }
};

class NoTalk : public TalkableRobot
{
public:
   void talk() override
   {
      cout << "Cannot Talk....." << "\n";
   }
};

// Strategy interface for flyable
class FlyableRobot
{
public:
   virtual void fly() = 0;
   virtual ~FlyableRobot() {}
};

class NormalFly: public FlyableRobot
{
public:
   void fly() override
   {
      cout << "Flying Normally...." << "\n";
   }
};

class NoFly : public FlyableRobot
{
public:
   void fly() override
   {
      cout << "Cannot fly...." << "\n";
   }
};

class Robot{
   protected:
     WalkableRobot* walkBehaviour;
     TalkableRobot* talkBehaviour;
     FlyableRobot* flyBehaviour;

   public:
     Robot(WalkableRobot* w, TalkableRobot *t, FlyableRobot *f){
       this->walkBehaviour = w;
       this->talkBehaviour = t;
       this->flyBehaviour  = f;
     }
     
     void walk(){
        walkBehaviour->walk();
     }

     void talk(){
      talkBehaviour->talk();
     }

     void fly(){
      flyBehaviour->fly();
     }

     virtual void projection()=0; //abstract method for subclasses
};


// Type A robots
class CompanionRobot : public Robot {
public:
   CompanionRobot(WalkableRobot *w, TalkableRobot* t, FlyableRobot *f): Robot(w,t,f) {}
   void projection() override {
      cout<< "displaying friendly companion stats ...."<<"\n";
   }    
};

// Type B robots
class WorkerRobot : public Robot {
public:
   WorkerRobot(WalkableRobot *w, TalkableRobot* t, FlyableRobot *f): Robot(w,t,f) {}
   void projection() override {
      cout<< "displaying worker Efficiency stats ...."<<"\n";
   }    
};

int main()
{
   Robot *robot1 = new CompanionRobot(new NormalWalk(), new NormalTalk(), new NoFly());
   robot1->walk();
   robot1->talk();
   robot1->fly();
   robot1->projection();

   cout<<"--------------------------------"<<"\n";
   Robot *robot2 = new WorkerRobot(new NoWalk(), new NoTalk(), new NormalFly());
   robot2->walk();
   robot2->talk();
   robot2->fly();
   robot2->projection();
}