#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <queue>
#include <set>
#include <stack>

using namespace std;

struct Subset {
    int parent;
    int rank;
};

struct Edge {
    int u, v, weight;

    friend ostream& operator<<(ostream& os, const Edge& edge) {
        os << "Edge(" << edge.u << ", " << edge.v << ", " << edge.weight << ")";
        return os;
    }
};

bool compareWeight(Edge a, Edge b) {
    return a.weight < b.weight;
}

void readFile(const string& filename, vector<Edge>& edges, vector<vector<int>>& AdjMatrixDistance, vector<vector<int>>& AdjMatrixCapacity, int& n) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error al abrir el archivo" << endl;
        return;
    }

    vector<vector<int>> auxAdjMatrixDistance;
    vector<vector<int>> auxAdjMatrixCapacity;
    string line;

    // Número de vértices
    if (getline(file, line)) {
        n = stoi(line);
    }

    getline(file, line); // Saltar línea

    // Matriz de adyacencia con distancias (Kruskal)
    for (int i = 0; i < n; i++) {
        vector<int> row;
        getline(file, line);
        int weight;
        size_t pos = 0;

        // Separar la línea por espacios para obtener cada valor
        while ((pos = line.find(' ')) != string::npos) {
            weight = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);
            row.push_back(weight);
        }
        if (!line.empty()) {
            row.push_back(stoi(line));
        }

        auxAdjMatrixDistance.push_back(row);
    }

    AdjMatrixDistance = auxAdjMatrixDistance;

    getline(file, line);

    // Matriz de adyadencia de capacidades (Ford-Fullkerson)
    for (int i = 0; i < n; i++) {
        vector<int> row;
        getline(file, line);
        int weight;
        size_t pos = 0;

        while ((pos = line.find(' ')) != string::npos) {
            weight = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);
            row.push_back(weight);
        }
        if (!line.empty()) {
            row.push_back(stoi(line));
        }

        auxAdjMatrixCapacity.push_back(row);
    }

    AdjMatrixCapacity = auxAdjMatrixCapacity;

    // Aristas de Kruskal
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (auxAdjMatrixDistance[i][j] != 0 && i < j) {
                edges.push_back({i, j, auxAdjMatrixDistance[i][j]});
            }
        }
    }

    file.close();
}

// -------------- KRUSKAL --------------

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

// -------------- FORD-FULLKERSON --------------

// s, nodo fuente
// t, nodo destino
// búsqueda en anchura para verificar que siga existiendo un camino aumentado desde s hasta t
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
            // verificar que el nodo no haya sido visitado y que la arista todavía tenga capacidad
            if (visited[v] == false && residualGraphAdjMatrix[u][v] > 0) {
                // retornar verdadero si se encuentra el nodo t
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

// regresa el flujo maximo en un grafo desde s hasta t
int ford_fullkerson(vector<vector<int>>& adjacencyMatrix, int s, int t) {
    int max_flow = 0;

    // se crea el grafo residual como una copia
    int n = adjacencyMatrix.size();
    vector<vector<int>> residualGraphAdjMatrix(n, vector<int>(n));

    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            residualGraphAdjMatrix[u][v] = adjacencyMatrix[u][v];
        }
    }

    // vector para saber quienes son los padres en la busqueda en anchura
    vector<int> parents(n, -1);

    int v, u;

    // mientras exista un camino aumentado
    while (BFS(residualGraphAdjMatrix, s, t, parents) == true) {
        int path_flow = INT_MAX;

        for (v = t;  v != s;  v = parents[v]) {
            u = parents[v];
            path_flow = min(path_flow, residualGraphAdjMatrix[u][v]);
        }

        // restar capacidad en las aristas normales y sumar en las aristas invertidas
        for (v = t;  v != s;  v = parents[v]) {
             u = parents[v];

            residualGraphAdjMatrix[u][v] -= path_flow;
            residualGraphAdjMatrix[v][u] += path_flow;
        }

        // acumular el flujo
        max_flow += path_flow;
    }

    return max_flow;
}


// -------------- TSP --------------

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

int main(int argc, char *argv[]) {
    string filename = argv[1];

    vector<Edge> edges;
    vector<vector<int>> AdjMatrixDistance;
    vector<vector<int>> AdjMatrixCapacity;
    int n;
    vector<Edge> arcs;

    readFile(filename, edges, AdjMatrixDistance, AdjMatrixCapacity, n);

    // 1. Kruskal
    vector<vector<int>> mstGraph = kruskalMST(edges, n, arcs);

    cout << "1. Kruskal:" << endl; 
    cout << "Arcos para conectar todo el grafo 1:" << endl;
    for (Edge arc : arcs) {
        cout << "(" << arc.u << ", " << arc.v << ")" << endl;
    }

    // 2. TSP
    cout << "\n2. TSP:" << endl; 
    cout << "Ruta que se debe de seguir (camino hamiltoniano) en el grafo 1:" << endl;
    christofidesTSP(mstGraph, AdjMatrixDistance);

    // 3. Ford-Fullkerson
    int max_flow = ford_fullkerson(AdjMatrixCapacity, 0, n - 1);

    cout << "\n3. Ford-Fullkerson:" << endl;
    cout << "Flujo maximo en el grafo 2: " << max_flow << endl;

    return 0;
}