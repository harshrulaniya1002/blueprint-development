 class MultiThreadedWebCrawler {
    unordered_map<string,int> vis;
    mutex mtx;
    public:

    void executeDfs(string url, HTMLParser *htmlParser){
        unique_lock<mutex> lock(mtx);
        if(vis.count(url)){
            return ;
        }
        //criticalsection
        vis[url]= 1;
        cout<<url<<"\n";
        l.unlock();

        vector<string>urls = htmlParser->getUrls(url);
        vector<thread> threads;
        // main thread for loop mei na jaake , andar recursive call k liye child threads call kara
        for(auto node: urls){
            threads.push_back(thread(MultiThreadedWebCrawler::executeDfs,this,node,htmlParser));
        }

        for(int i=0;i<threads.size();i++){
            threads[i].join();
        }
    }
 };

 //issue race condition
//  a->b->c 
//  a->d->c   (here thread will created 2 duplicates)
// also if graph is like a->b->c->d->e , here n numbers of threads will be created , here lots of context swiwtching will be done