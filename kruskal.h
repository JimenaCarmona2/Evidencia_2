#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <vector>
using namespace std;

struct Edge {
    int u, v, weight;
};

vector<vector<int>> kruskalMST(const vector<Edge>& edges, int V, vector<Edge>& arcs);

#endif
