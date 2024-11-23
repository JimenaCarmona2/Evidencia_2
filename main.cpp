#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "kruskal.h"
#include "ford_fullkerson.h"

using namespace std;

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

int main(int argc, char *argv[]) {
    string filename = argv[1];

    vector<Edge> edges;
    vector<vector<int>> AdjMatrixDistance;
    vector<vector<int>> AdjMatrixCapacity;
    int n;
    vector<Edge> arcs;

    readFile(filename, edges, AdjMatrixDistance, AdjMatrixCapacity, n);

    // 1. Kruskal
    kruskalMST(edges, n, arcs);

    cout << "Arcos para conectar todo el grafo:" << endl;
    for (Edge arc : arcs) {
        cout << "(" << arc.u << ", " << arc.v << ")" << endl;
    }

    // 3. Ford-Fullkerson
    int max_flow = ford_fullkerson(AdjMatrixCapacity, 0, n - 1);

    cout << "Flujo máximo en el grafo:" << max_flow << endl;

    return 0;
}