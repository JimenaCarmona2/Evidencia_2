#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <set>
#include "kruskal.h"

using namespace std;

// se utiliza para encontrar las aristas 'minimum weight perfect matching' y así conectar los nodos
// que tienen un número impar de aristas
vector<Edge> minimumWeightPerfectMatching(int n, vector<Edge>& edges, set<pair<int,int>> mstEdgesSet) {
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
        if (!inMatching[u] && !inMatching[v] && mstEdgesSet.find({u, v}) == mstEdgesSet.end()) {
            matching.push_back(edge);
            inMatching[u] = inMatching[v] = true;
        }
    }

    return matching;
}

vector<int> findEulerianCycle(vector<Edge>& mstEdges, int start) {
    // Lista de adyacencia de mstEdges
    vector<vector<pair<int, bool>>> adjList(mstEdges.size()); // Bool para indicar visita

    // Añadir aristas para grafos no dirigidos
    for (const auto& edge : mstEdges) {
        adjList[edge.u].emplace_back(edge.v, false);
        adjList[edge.v].emplace_back(edge.u, false);
    }

    stack<int> path;
    vector<int> finalPath;

    // Iniciamos el recorrido con el primer nodo 
    path.push(start);
    int currNode = start;

    // Si hay nodos en el camino actual (path)
    while (!path.empty()) {
        bool hasUnusedEdge = false;
        // Buscar una arista no usada desde el nodo actual
        for (auto& neighbor : adjList[currNode]) {
            // Marcar las aristas como usadas
            if (!neighbor.second) {
                neighbor.second = true;

                // Marcar la arista de vuelta (simetría)
                for (auto& backEdge : adjList[neighbor.first]) {
                    if (backEdge.first == currNode && !backEdge.second) {
                        backEdge.second = true;
                        break;
                    }
                }

                path.push(currNode); // Guardar el nodo actual en la ruta
                currNode = neighbor.first; // Moverse al siguiente nodo
                hasUnusedEdge = true;
                break;
            }
        }

        if (!hasUnusedEdge) {
            // Si no hay más vecinos, añadimos el nodo actual al ciclo y volvemos en la pila
            finalPath.push_back(currNode);
            currNode = path.top();
            path.pop();
        }
    }

    return finalPath;
}

// Convierte el ciclo Euleriano en un ciclo Hamiltoniano
vector<int> deleteRepeatedNodes(const vector<int>& eulerianCycle) {
    vector<int> finalPath;
    vector<bool> visited(eulerianCycle.size(), false); // Nodos visitados en falso por default

    for(int node : eulerianCycle) {
        // Si no se ha visitado se agrega el camino
        if (!visited[node]) {
            finalPath.push_back(node);
            visited[node] = true;
        }
    }

    // Regresa al nodo inicial para completarlo
    finalPath.push_back(finalPath[0]);

    return finalPath;
}

// **SOLAMENTE FUNCIONA PARA UN GRAFO COMPLETO NO DIRIGIDO**
// aproximación de solución para el problema TSP, tiene como entrada una matriz de adyacencia del MST de un grafo completo
// y el grafo original completo
void christofidesTSP(vector<vector<int>>& mstGraph, vector<vector<int>>& originalGraph) {

    // vector de nodos con número impar de aristas
    vector<int> oddNumberEdgesNodes;

    int n = mstGraph.size();

    // Aristas de mstGraph
    vector<Edge> mstEdges;
    set<pair<int, int>> mstEdgesSet;
    for (int u = 0; u < n; u++) {
        for (int v = u; v < n; v++) {
            if (mstGraph[u][v] > 0) {
                mstEdges.push_back({u, v, mstGraph[u][v]});
                mstEdgesSet.insert({u, v});
                mstEdgesSet.insert({v, u});
            }
        }
    }

    // encontrar los nodos que tienen un número de aristas impar en la matriz de adyacencia del MST
    for (int u = 0; u < n; u++) {
        int nodeCount = 0;

        for (int v = 0; v < n; v++) {
            if (mstGraph[u][v] > 0) {
                nodeCount++;
            }
        }

        if (nodeCount % 2 != 0) {
            oddNumberEdgesNodes.push_back(u);
        }
    }

    // crear un vector de las aristas del grafo original solamente con los nodos que tienen un numero impar
    // de aristas en el MST
    vector<Edge> minWeightEdges;

    // por cada nodo con no. impar de aristas en el MST agregar su arista a minWeightEdges
    for (int i = 0; i < oddNumberEdgesNodes.size(); i++) {
        for (int j = 0; j < oddNumberEdgesNodes.size(); j++) {
            int u = oddNumberEdgesNodes[i];
            int v = oddNumberEdgesNodes[j];

            // si existe una arista se agrega a minWeightEdges
            if (originalGraph[u][v] > 0) {
                minWeightEdges.push_back({u, v, originalGraph[u][v]});
            }
        }
    }

    // algoritmo de blossom para encontrar las aristas de minimo costo que conectan a los nodos restantes
    vector<Edge> matching = minimumWeightPerfectMatching(n, minWeightEdges, mstEdgesSet);

    // Agregar las aristas matching (matching) a las aristas de la matriz de adyacencia de MST (edges)
    for (Edge edge : matching) {
        mstEdges.push_back(edge);
    }

    // Encontrar el ciclo Euleriano
    vector<int> eulerianCycle = findEulerianCycle(mstEdges, 0);

    // Eliminar aristas con nodos repetidos (ciclo Hamiltoniano)
    vector<int> finalPath = deleteRepeatedNodes(eulerianCycle);

    cout << "Ciclo Euleriano: ";
    for (int vertex : eulerianCycle) {
        cout << vertex << " ";
    }
    cout << endl;

    cout << "Ciclo Hamiltoniano: ";
    for (int vertex : finalPath) {
        cout << vertex << " ";
    }
    cout << endl;

}