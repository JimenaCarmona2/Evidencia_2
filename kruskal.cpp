#include <iostream>
#include <vector>
#include <fstream>
#include <string>

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

void readFile(const string& filename, vector<Edge>& edges, vector<vector<int>>& AdjMatrixDistance) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error al abrir el archivo" << endl;
        return;
    }

    vector<vector<int>> auxAdjMatrixDistance;
    string line;
    int n;

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

    readFile(filename, edges, AdjMatrixDistance);

    return 0;
}