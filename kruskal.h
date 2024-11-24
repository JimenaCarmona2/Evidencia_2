#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <vector>
using namespace std;

struct Edge {
    int u, v, weight;

    friend ostream& operator<<(ostream& os, const Edge& edge) {
        os << "Edge(" << edge.u << ", " << edge.v << ", " << edge.weight << ")";
        return os;
    }
};


bool compareWeight(Edge a, Edge b);

vector<vector<int>> kruskalMST(const vector<Edge>& edges, int V, vector<Edge>& arcs);

#endif
