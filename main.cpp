#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "kruskal.h"
#include "ford_fullkerson.h"
#include "christofides.h"

using namespace std;

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
    string filename = argv[1];

    vector<Edge> edges;
    vector<vector<int>> AdjMatrixDistance;
    int n;
    vector<Edge> arcs;

    readFile(filename, edges, AdjMatrixDistance, n);

    // 1. Kruskal
    vector<vector<int>> mstGraph = kruskalMST(edges, n, arcs);

    cout << "Arcos para conectar todo el grafo:" << endl;
    for (Edge arc : arcs) {
        cout << "(" << arc.u << ", " << arc.v << ")" << endl;
    }


    // 2. TSP
    christofidesTSP(mstGraph, AdjMatrixDistance);


    // 3. Ford-Fullkerson
    int max_flow = ford_fullkerson(AdjMatrixDistance, 0, n - 1);

    cout << "Flujo máximo en el grafo:" << max_flow << endl;


    return 0;
}