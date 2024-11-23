#include <iostream>
#include "kruskal.h"

using namespace std;

void christofidesTSP(vector<vector<int>>& mstGraph) {

    // arreglo para guardar los nodos que tienen un número impar de aristas
    vector<int> oddNumberEdgesNodes;

    int n = mstGraph.size();

    int nodeCount;
    // encontrar los nodos que tienen un número impar de aristas
    for (int u = 0; u < n; u++) {

        nodeCount = 0;
        
        for (int v = 0; v < n; v++) {
            if (mstGraph[u][v] > 0) {
                nodeCount++;
            }  
        }

        if (nodeCount % 2 != 0) {
            oddNumberEdgesNodes.push_back(u);
        }
    }

    // imprimir los nodos que tienen un número impar de aristas
    cout << "Nodos con un número impar de aristas: ";
    for (int node : oddNumberEdgesNodes) {
        cout << node << " ";
    }
    cout << endl;

}

