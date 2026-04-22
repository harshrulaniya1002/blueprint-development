//children ko parallely execute karenge
void executeBFS()
{
    string t = q.front();
    cout<<t<<"\n";
    q.pop();

    vector<string> urls =  htmlParser->getUrls(t);
    for(auto node: urls){
        if(!vis.count(node)){
            vis[node]=1;
            q.push(node);
        }
    }
}


//apporach-2

void executeBFS() //4 threads
{
    while(1){
        unique_lock<mutex> l(mtx);
        cv.wait_for(l,chrono::seconds(1),[&]{return !q.empty();}); //sleep for 1 sec
        if(q.empty()) break;
        string t=q.front();
        cout<<t<<"\n";
        q.pop();
        l.unlock();

        vector<string>urls = htmlParser->getUrls(t);
        for(auto node: urls){
            unqiue_locks<mutex> l(mtx);
            if(!vis.count(node)){
                vis[node]=1;
                q.push(node);
            }
        }
        v.notify_all();
    }

}