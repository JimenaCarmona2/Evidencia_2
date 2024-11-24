#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "kruskal.h"

struct Subset {
    int parent;
    int rank;
};

// Función para encontrar el padre de u
int find(Subset subsets[], int u) {
    if (subsets[u].parent != u) {
        subsets[u].parent = find(subsets, subsets[u].parent);
    }
    return subsets[u].parent;
}

// Función para hacer la unión de dos conjuntos (árboles) según el rank
void mergeUnion(Subset subsets[], int u, int v) {
    int aux_u = find(subsets, u);
    int aux_v = find(subsets, v);

    if (aux_u != aux_v) {
        if (subsets[aux_u].rank < subsets[aux_v].rank) {
            subsets[aux_u].parent = aux_v;
        } else if (subsets[aux_u].rank > subsets[aux_v].rank) {
            subsets[aux_v].parent = aux_u;
        } else {
            subsets[aux_v].parent = aux_u;
            subsets[aux_u].rank++;
        }
    }
}

bool compareWeight(Edge a, Edge b) {
    return a.weight < b.weight;
}

vector<vector<int>> kruskalMST(const vector<Edge>& edges, int V, vector<Edge>& arcs) {
    vector<Edge> res;  // Resultado MST
    int weightMST = 0;

    // Ordenar las aristas por peso
    vector<Edge> sortedEdges = edges; // No modifica el original
    sort(sortedEdges.begin(), sortedEdges.end(), compareWeight);

    Subset* subsets = new Subset[V];

    for (int i = 0; i < V; ++i) {
        subsets[i].parent = i; // Son padres de sí mismos al iniciar
        subsets[i].rank = 0;
    }

    // Matriz de adyacencia para el MST
    vector<vector<int>> mstMatrix(V, vector<int>(V, 0));
    for (const Edge& edge : sortedEdges) {
        int u = find(subsets, edge.u);
        int v = find(subsets, edge.v);

        // Si u y v están en diferentes conjuntos, se unen
        if (u != v) {
            res.push_back(edge);
            weightMST += edge.weight;
            mergeUnion(subsets, u, v);

            mstMatrix[edge.u][edge.v] = edge.weight;
            mstMatrix[edge.v][edge.u] = edge.weight;

            arcs.push_back(edge);
        }
    }

    delete[] subsets;
    return mstMatrix;
}