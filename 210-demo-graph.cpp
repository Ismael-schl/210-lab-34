//Lab 34 | COMSC 210 | Ismael Hadi
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

const int SIZE = 13;

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

        // add edges to the directed graph
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

int main() {
    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        {0,1,15},{0,2,5},{0,3,11},{2,3,7},{2,5,8},{7,3,4},
        {5,11,9},{2,7,6},{1,9,2},{1,10,7},{3,8,6},{10,12,5}
    };

    // Creates graph
    Graph graph(edges);

    // Prints adjacency list representation of graph
    graph.printGraph();
    graph.BFS(0);
    graph.DFS(0);



    return 0;
}