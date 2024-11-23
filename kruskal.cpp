#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

struct Edge {
    int u, v, weight;
};

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

void kruskalMST(const vector<Edge>& edges, int V, vector<Edge>& arcs) {
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

            arcs.push_back(edge);
        }
    }

    cout << "\nMatriz de adyacencia del MST:\n";
    ofstream outfile("MST.txt");
    if (outfile.is_open()) {
        for (const auto& row : mstMatrix) {
            for (int weight : row) {
                cout << weight << " ";
                outfile << weight << " ";
            }
            cout << endl;
            outfile << endl;
        }
        outfile.close();
    } else {
        cerr << "Error" << endl;
    }

    delete[] subsets;
}

void readFile(const string& filename, vector<Edge>& edges, vector<vector<int>>& AdjMatrixDistance, int& n) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error al abrir el archivo" << endl;
        return;
    }

    vector<vector<int>> auxAdjMatrixDistance;
    string line;

    // Leer número de vértices
    if (getline(file, line)) {
        n = stoi(line);
    }

    getline(file, line); // Saltar línea

    // Leer la matriz de adyacencia 
    while (getline(file, line)) {
        vector<int> row;
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

    // Aristas del grafo
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (auxAdjMatrixDistance[i][j] != 0 && i < j) {
                edges.push_back({i, j, auxAdjMatrixDistance[i][j]});
            }
        }
    }

    file.close();
}

int main(int argc, char *argv[]) {
    string filename = "input.txt";

    vector<Edge> edges;
    vector<vector<int>> AdjMatrixDistance;
    int n;
    vector<Edge> arcs;

    readFile(filename, edges, AdjMatrixDistance, n);
    kruskalMST(edges, n, arcs);

    for (Edge arc : arcs) {
        cout << "(" << arc.u << ", " << arc.v << ")" << endl;
    }

    return 0;
}