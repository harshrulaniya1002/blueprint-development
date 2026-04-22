//producer
#include<iostream>
#include<mutex>
using namespace std;

mutex mtx;
condition_variable cv;
// void pushToMessageQueue(int message){
//     lock_guard<mutex> lock(mtx);
//     messageQueue.push(message);
// }

// void consumerFromMessageQueue(){
//     while(1){
//         lock_guard<mutex> lock(mtx);
//         if(!messageQueue.empty()){
//             cout<<messageQueue.front();
//             messageQueue.pop();
//         }
//     }
// }


//consumer lagatar check kar rha, to lock release hi nhi karega,
// so will find a way jisme consumer notify hoga jab bhi message queue message aayega

void pushToMessageQueue(int message){
    unique_lock<mutex> lock(mtx);
    messageQueue.push(message);
    cv.notify_one();
}

// cv.wait() will make consumer thread to sleep and also release the lock, not blocking the producer from pushing the message.

void consumerFromMessageQueue(){
    while(1){
        unique_lock<mutex> lock(mtx);
        cv.wait(lock,[] {return !messageQueue.empty();});
        messageQueue.pop();
    }
}