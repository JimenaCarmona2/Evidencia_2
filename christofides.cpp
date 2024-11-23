#include <iostream>
#include <vector>
#include <algorithm>
#include "kruskal.h"

using namespace std;

// se utiliza para encontrar las aristas 'minimum weight perfect matching' y así conectar los nodos
// que tienen un número impar de aristas
vector<Edge> minimumWeightPerfectMatching(int n, vector<Edge>& edges) {
    // vector de aristas que conectan los nodos con número de aristas impares 
    vector<Edge> matching;

    // vector para saber si las aristas ya fueron conectadas
    vector<bool> inMatching(n, false);

    // ordenar las aristas
    sort(edges.begin(), edges.end(), compareWeight);

    // por cada arista tratar de conectar los nodos
    for (Edge edge : edges) {
        int u = edge.u;
        int v = edge.v;

        // si los nodos no han sido agregados a la lista se agregan
        if (!inMatching[u] && !inMatching[v]) {
            matching.push_back(edge);
            inMatching[u] = inMatching[v] = true;
        }
    }

    return matching;
}

// **SOLAMENTE FUNCIONA PARA UN GRAFO COMPLETO**
// aproximación de solución para el problema TSP, tiene como entrada una matriz de adyacencia del MST de un grafo completo
// y el grafo original completo
void christofidesTSP(vector<vector<int>>& mstGraph, vector<vector<int>>& originalGraph) {

    // vector de nodos con número impar de aristas
    vector<int> oddNumberEdgesNodes;

    int n = mstGraph.size();

    // encontrar los nodos que tienen un número de aristas impar en la matriz de adyacencia del MST
    for (int u = 0; u < n; u++) {
        int nodeCount = 0;

        for (int v = 0; v < n; v++) {
            if (mstGraph[u][v] > 0) {
                nodeCount++;
            }
        }
        cout << "odd number node" << endl;
        if (nodeCount % 2 != 0) {
            oddNumberEdgesNodes.push_back(u);
            cout << u << endl;
        }
    }

    // crear un vector de las aristas del grafo original solament con los nodos que tienen un numero impar
    // de aristas en el MST
    vector<Edge> minWeightEdges;

    // por cada nodo con no. impar de aristas en el MST agregar su arista a minWeightEdges
    for (int i = 0; i < oddNumberEdgesNodes.size(); i++) {
        for (int j = 0; j < oddNumberEdgesNodes.size(); j++) {
            int u = oddNumberEdgesNodes[i];
            int v = oddNumberEdgesNodes[j];

            // si existe una arista se agrega un numero de 
            if (originalGraph[u][v] > 0) {
                minWeightEdges.push_back({u, v, originalGraph[u][v]});
            }
        }
    }

    // algoritmo de blossom para encontrar las aristas de minimo costo que conectan a los nodos restantes
    vector<Edge> matching = minimumWeightPerfectMatching(n, minWeightEdges);

    for (Edge edge : matching) {
        cout << edge.u << ", " << edge.v << endl;
    }

    // falta agregar las aristas matching a la matriz de adyacencia de MST, encontrar el camino euleriano y quitar aristas que tengan nodos repetidos
}