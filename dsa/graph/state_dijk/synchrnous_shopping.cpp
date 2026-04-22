// dijkstra state 
#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'shop' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. INTEGER k
 *  3. STRING_ARRAY centers
 *  4. 2D_INTEGER_ARRAY roads
 */




int shop(int n, int k, vector<string> centers, vector<vector<int>> roads) {

vector<int>maskF(n+1);
int fullmsk = ((1<<k)-1);

for(int i=1;i<=n;i++){
    vector<string>temp = split(centers[i-1]);
    int cnt = stoi(temp[0]);
    int msk=0;
    for(int j=1;j<=cnt;j++){
        int typ = stoi(temp[j]);
        msk = msk | (1<<(typ-1));
    }
    maskF[i]=msk;
}

vector<vector<pair<int,int>>>adj(n+1);
for(auto it: roads){
    int u=it[0];
    int v=it[1];
    int wt=it[2];
    
    adj[u].push_back({v,wt});
    adj[v].push_back({u,wt});
}

priority_queue<pair<int,pair<int,int>>, vector<pair<int,pair<int,int>>>, greater<pair<int,pair<int,int>>>>pq;
vector<vector<int>>dist(n+1, vector<int>(fullmsk+1,INT_MAX));

pq.push({0,{maskF[1],1}});
dist[1][maskF[1]]=0;

while(!pq.empty()){
    auto it = pq.top();
    pq.pop();
    
    int time = it.first;
    int currmsk = it.second.first;
    int node = it.second.second;
    
    if(time > dist[node][currmsk]) continue;
    
    for(auto edge : adj[node]){
        int v = edge.first;
        int wt = edge.second;
        int nextmsk = currmsk | maskF[v];
        if(time + wt < dist[v][nextmsk]){
            dist[v][nextmsk] = time + wt;
            pq.push({dist[v][nextmsk], {nextmsk,v}});
        }
    }
}


int ans=INT_MAX;
for(int i=0;i<=fullmsk;i++){
    for(int j=0;j<=fullmsk;j++){
        if((i|j) == fullmsk){
            if(dist[n][i] != INT_MAX && dist[n][j] != INT_MAX){
                ans = min(ans, max(dist[n][i],dist[n][j]));
            }
        }
    }
}

return ans;

}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);

    int m = stoi(first_multiple_input[1]);

    int k = stoi(first_multiple_input[2]);

    vector<string> centers(n);

    for (int i = 0; i < n; i++) {
        string centers_item;
        getline(cin, centers_item);

        centers[i] = centers_item;
    }

    vector<vector<int>> roads(m);

    for (int i = 0; i < m; i++) {
        roads[i].resize(3);

        string roads_row_temp_temp;
        getline(cin, roads_row_temp_temp);

        vector<string> roads_row_temp = split(rtrim(roads_row_temp_temp));

        for (int j = 0; j < 3; j++) {
            int roads_row_item = stoi(roads_row_temp[j]);

            roads[i][j] = roads_row_item;
        }
    }

    int res = shop(n, k, centers, roads);

    fout << res << "\n";

    fout.close();

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
