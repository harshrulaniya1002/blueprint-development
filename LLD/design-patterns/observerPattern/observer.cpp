#include<bits/stdc++.h>
using namespace std;

class ISubscriber {
public:
    virtual void update() = 0;
    virtual ~ISubscriber() {}
};

class IChannel {
public:
    virtual void subscribe(ISubscriber* subscriber) = 0;
    virtual void unsubscribe(ISubscriber* subscriber) = 0;
    virtual void notifySubscribers() = 0;
    virtual ~IChannel() {}
};

class Channel : public IChannel {
private:
    vector<ISubscriber*> subscribers;
    string name;
    string latestVideo; //latest uploaded video title
public:
    Channel(const string& name){
        this->name = name;
    }

    void subscribe(ISubscriber* subscriber) override {
        if (find(subscribers.begin(), subscribers.end(), subscriber) == subscribers.end()) {
            subscribers.push_back(subscriber);
        }
    }

    void unsubscribe(ISubscriber* subscriber) override {
        auto it = find(subscribers.begin(), subscribers.end(), subscriber);
        if (it != subscribers.end()) {
            subscribers.erase(it);
        }
    }

    void notifySubscribers() override {
        for (ISubscriber* subscriber : subscribers) {
            subscriber->update();
        }
    }

    void uploadVideo(const string& title) {
        latestVideo = title;
        cout<<"\n[" << name << "] New video uploaded: " << latestVideo << endl;
        notifySubscribers();
    }

    string getVideoData(){
        return "\nCheckout our new video: " + latestVideo;
    }
};

class Subscriber : public ISubscriber {
private:
    string name;
    Channel* channel;
public:
    Subscriber(const string& name, Channel* channel) {
        this->name = name;
        this->channel = channel;
    }

    void update() override {
        cout << "\n[" << name << "] " << channel->getVideoData() << endl;
    }
};


int main(){
    Channel* channel = new Channel("Tech Channel");
    Subscriber* sub1 = new Subscriber("Alice", channel);
    Subscriber* sub2 = new Subscriber("Bob", channel);

    channel->subscribe(sub1);
    channel->subscribe(sub2);

    channel->uploadVideo("Observer Pattern in C++");

    channel->unsubscribe(sub1);

    channel->uploadVideo("Design Patterns Explained");

    delete sub1;
    delete sub2;
    delete channel;

    return 0;
}