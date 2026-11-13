//Lab 34 | COMSC 210 | Ismael Hadi
// Step 3: Global airline route network built on the weighted graph
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
using namespace std;

const int SIZE = 13;  // vertices 0 through 12

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;  // Creates alias 'Pair' for the pair<int,int> data type

class Graph {
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;

    // Graph Constructor
    Graph(vector<Edge> const &edges) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);

        // add edges to the graph
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));
            // for an undirected graph, add an edge from dest to src also
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    // Print the graph's adjacency list
    void printGraph() {
        cout << "Graph's adjacency list:" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << " --> ";
            for (Pair v : adjList[i])
                cout << "(" << v.first << ", " << v.second << ") ";
            cout << endl;
        }
    }

    // Depth-first search (iterative, stack-based) starting from vertex start
    void DFS(int start) {
        vector<bool> visited(adjList.size(), false);
        stack<int> s;

        cout << "DFS starting from vertex " << start << ":" << endl;
        s.push(start);

        while (!s.empty()) {
            int v = s.top();
            s.pop();

            // a vertex can be pushed more than once, so skip it if already visited
            if (visited[v])
                continue;

            visited[v] = true;
            cout << v << " ";

            // push every unvisited neighbor; the last one pushed is explored first
            for (Pair neighbor : adjList[v])
                if (!visited[neighbor.first])
                    s.push(neighbor.first);
        }
        cout << endl;
    }

    // Breadth-first search (queue-based) starting from vertex start
    void BFS(int start) {
        vector<bool> visited(adjList.size(), false);
        queue<int> q;

        cout << "BFS starting from vertex " << start << ":" << endl;
        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            cout << v << " ";

            // mark neighbors visited when enqueued so each vertex enters the queue once
            for (Pair neighbor : adjList[v]) {
                if (!visited[neighbor.first]) {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);
                }
            }
        }
        cout << endl;
    }
};

// Application layer: vertices are airports, edges are nonstop routes,
// and edge weights are approximate flight times in hours.
class AirlineNetwork {
private:
    Graph routes;          // the underlying weighted graph
    vector<string> code;   // airport code for each vertex ("" = airport closed)
    vector<string> city;   // city name for each vertex

    // true if the airport still has at least one route
    bool isServed(int v) const {
        return !routes.adjList[v].empty();
    }

    // e.g. "SFO (San Francisco)"
    string label(int v) const {
        return code[v] + " (" + city[v] + ")";
    }

public:
    AirlineNetwork(vector<Edge> const &edges) : routes(edges) {
        code = {"SFO", "SYD", "JFK", "LHR", "", "CDG", "",
                "KEF", "DXB", "MEL", "SIN", "BOM", "HKG"};
        city = {"San Francisco", "Sydney", "New York", "London", "", "Paris", "",
                "Reykjavik", "Dubai", "Melbourne", "Singapore", "Mumbai", "Hong Kong"};
    }

    // Lists every airport that still has service and its nonstop routes
    void printRouteMap() {
        cout << "Global Airline Route Network" << endl;
        cout << "========================================" << endl;
        for (int v = 0; v < SIZE; v++) {
            if (!isServed(v))
                continue;
            cout << label(v) << " has nonstop flights to:" << endl;
            for (Pair route : routes.adjList[v])
                cout << "   -> " << label(route.first)
                     << " (" << route.second << " hrs)" << endl;
        }

        cout << "Closed airports (no routes):";
        for (int v = 0; v < SIZE; v++)
            if (!isServed(v))
                cout << " #" << v;
        cout << endl;
    }

    // DFS: follows one chain of connections as deep as it goes before backtracking
    void exploreItineraries(int start) {
        vector<bool> visited(SIZE, false);
        stack<int> s;

        cout << "Itinerary Explorer (DFS) from " << label(start) << endl;
        cout << "Purpose: Following one chain of connections as deep as possible" << endl;
        cout << "         before backtracking (multi-stop trip planning)" << endl;
        cout << "========================================" << endl;

        s.push(start);
        while (!s.empty()) {
            int v = s.top();
            s.pop();

            // an airport can be pushed more than once, so skip it if already explored
            if (visited[v])
                continue;

            visited[v] = true;
            cout << "Exploring " << label(v) << endl;

            for (Pair route : routes.adjList[v]) {
                if (!visited[route.first]) {
                    cout << "   -> Next leg available: " << label(route.first)
                         << " (" << route.second << " hrs)" << endl;
                    s.push(route.first);
                }
            }
        }
    }

    // BFS: groups airports by the number of flights needed from the start
    void fewestStops(int start) {
        vector<bool> visited(SIZE, false);
        vector<int> flights(SIZE, 0);   // number of flights from start
        vector<int> hours(SIZE, 0);     // total flight time along that route
        vector<int> from(SIZE, -1);     // airport we flew in from
        queue<int> q;

        cout << "Fewest-Connection Itineraries (BFS) from " << label(start) << endl;
        cout << "Purpose: Finding how many flights it takes to reach each city" << endl;
        cout << "         (fewest stops, not necessarily the fastest trip)" << endl;
        cout << "========================================" << endl;

        visited[start] = true;
        q.push(start);
        int currentLevel = -1;

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            // BFS visits airports in order of flight count, so print a header
            // each time the count changes
            if (flights[v] != currentLevel) {
                currentLevel = flights[v];
                if (currentLevel == 0)
                    cout << "Origin:" << endl;
                else if (currentLevel == 1)
                    cout << "Nonstop (1 flight):" << endl;
                else
                    cout << currentLevel - 1 << " connection"
                         << (currentLevel - 1 == 1 ? "" : "s")
                         << " (" << currentLevel << " flights):" << endl;
            }

            cout << "   " << label(v);
            if (from[v] != -1)
                cout << " - " << hours[v] << " hrs total, via " << code[from[v]];
            cout << endl;

            for (Pair route : routes.adjList[v]) {
                int next = route.first;
                if (!visited[next]) {
                    visited[next] = true;
                    flights[next] = flights[v] + 1;
                    hours[next] = hours[v] + route.second;
                    from[next] = v;
                    q.push(next);
                }
            }
        }
    }
};

int main() {
    // Creates a vector of route edges/flight times
    vector<Edge> edges = {
        // (x, y, w) —> route between airport x and airport y taking w hours
        // Changes from Step 1: nodes 4 and 6 deleted, nodes 7-12 added, weights changed
        {0,1,15},{0,2,5},{0,3,11},{2,3,7},{2,5,8},{7,3,4},
        {5,11,9},{2,7,6},{1,9,2},{1,10,7},{3,8,6},{10,12,5}
    };

    // Creates the airline network on top of the graph
    AirlineNetwork airline(edges);

    airline.printRouteMap();
    cout << endl;
    airline.exploreItineraries(0);   // vertex 0 = SFO hub
    cout << endl;
    airline.fewestStops(0);

    return 0;
}