#include <bits/stdc++.h>
using namespace std;

// You are given a road network with n cities connected by m bidirectional roads.
// Each city has a fuel price, and each road requires a certain amount of fuel to travel.
// You drive a car that has a fuel tank with limited capacity.
// Your goal is to find the minimum cost to travel from a starting city to a destination city.

struct State {
    int cost;
    int city;
    int fuel;

    bool operator>(const State &other) const {
        return cost > other.cost;
    }
};

int main() {

    int n, m;
    cin >> n >> m;

    vector<int> price(n);
    for(int i = 0; i < n; i++)
        cin >> price[i];

    vector<vector<pair<int,int>>> adj(n);

    for(int i = 0; i < m; i++) {
        int u, v, d;
        cin >> u >> v >> d;

        adj[u].push_back({v, d});
        adj[v].push_back({u, d});
    }

    int q;
    cin >> q;

    while(q--) {

        int capacity, start, target;
        cin >> capacity >> start >> target;

        vector<vector<int>> dist(n, vector<int>(capacity + 1, INT_MAX));

        priority_queue<State, vector<State>, greater<State>> pq;

        pq.push({0, start, 0});
        dist[start][0] = 0;

        int ans = -1;

        while(!pq.empty()) {

            auto cur = pq.top();
            pq.pop();

            int cost = cur.cost;
            int city = cur.city;
            int fuel = cur.fuel;

            if(city == target) {
                ans = cost;
                break;
            }

            if(cost > dist[city][fuel]) continue;

            // Option 1: Buy fuel
            if(fuel < capacity) {
                int newCost = cost + price[city];

                if(dist[city][fuel + 1] > newCost) {
                    dist[city][fuel + 1] = newCost;
                    pq.push({newCost, city, fuel + 1});
                }
            }

            // Option 2: Travel to neighbors
            for(auto &edge : adj[city]) {

                int nextCity = edge.first;
                int fuelNeeded = edge.second;

                if(fuel >= fuelNeeded) {

                    if(dist[nextCity][fuel - fuelNeeded] > cost) {
                        dist[nextCity][fuel - fuelNeeded] = cost;

                        pq.push({cost, nextCity, fuel - fuelNeeded});
                    }
                }
            }
        }

        if(ans == -1)
            cout << "impossible\n";
        else
            cout << ans << "\n";
    }
}