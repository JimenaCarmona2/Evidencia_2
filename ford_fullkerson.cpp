#include <iostream>
#include <queue>
#include "ford_fullkerson.h"

// s, nodo fuente
// t, nodo destino
bool BFS(vector<vector<int>>& residualGraphAdjMatrix, int s, int t, vector<int>& parents) {
    const int n = residualGraphAdjMatrix.size();
    queue<int> q;
    bool visited[n];

    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    q.push(s);
    parents[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (visited[v] == false && residualGraphAdjMatrix[u][v] > 0) {
                if (v == t) {
                    parents[v] = u;
                    return true;
                }

                visited[v] = true;
                parents[v] = u;
                q.push(v);
            }
        }
    }

    return false;
}

int ford_fullkerson(vector<vector<int>>& adjacencyMatrix, int s, int t) {
    int max_flow = 0;

    int n = adjacencyMatrix.size();
    vector<vector<int>> residualGraphAdjMatrix(n, vector<int>(n));

    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            residualGraphAdjMatrix[u][v] = adjacencyMatrix[u][v];
        }
    }

    vector<int> parents(n, -1);

    int v, u;

    while (BFS(residualGraphAdjMatrix, s, t, parents) == true) {
        int path_flow = INT_MAX;

        for (v = t;  v != s;  v = parents[v]) {
            u = parents[v];
            path_flow = min(path_flow, residualGraphAdjMatrix[u][v]);
        }

        for (v = t;  v != s;  v = parents[v]) {
             u = parents[v];

            residualGraphAdjMatrix[u][v] -= path_flow;
            residualGraphAdjMatrix[v][u] += path_flow;
        }

        max_flow += path_flow;
    }

    return max_flow;
}