// 3. Genera un grafo con pesos negativos y verifica si Bellman–Ford detecta ciclos negativos.
#include <iostream>
#include <vector>
using namespace std;

// Algoritmo de Bellman–Ford: calcula distancias mínimas desde origen
// y detecta ciclo negativo (retorna {-1} si existe).
vector<int> bellmanFord(int numVertices, vector<vector<int>>& aristas, int origen) {
    // Distancia inicial a todos los vértices: infinito (1e8 como valor grande)
    const int INF = 1e8;
    vector<int> distancia(numVertices, INF);
    distancia[origen] = 0;

    // Relajamos todas las aristas numVertices veces.
    // La iteración extra (i == numVertices - 1) permite detectar ciclo negativo.
    for (int i = 0; i < numVertices; i++) {
        for (const vector<int>& arista : aristas) {
            int u = arista[0];
            int v = arista[1];
            int peso = arista[2];

            if (distancia[u] != INF && distancia[u] + peso < distancia[v]) {
                // Si se puede relajar en la última pasada, hay ciclo negativo
                if (i == numVertices - 1)
                    return {-1};

                // Actualizar la distancia más corta hacia 'v'
                distancia[v] = distancia[u] + peso;
            }
        }
    }

    return distancia;
}

int main() {
    // Número de vértices del grafo
    int numVertices = 5;

    // Lista de aristas base (se duplicarán para hacer el grafo no dirigido)
    // {u, v, peso}
    vector<vector<int>> aristasBase = {
        {1, 3, 2},
        {4, 3, 5},
        {2, 4, 1},
        {1, 2, 1},
        {0, 1, 10}
    };

    // Convertir a grafo no dirigido: agregar arista inversa de cada una
    vector<vector<int>> aristas;
    for (const auto& arista : aristasBase) {
        aristas.push_back({arista[0], arista[1], arista[2]});  // u → v
        aristas.push_back({arista[1], arista[0], arista[2]});  // v → u (inversa)
    }

    // Solicitamos al usuario el vértice de origen
    int origen;
    cout << "Ingrese el vértice origen (0-" << numVertices - 1 << "): ";
    cin >> origen;

    if (origen < 0 || origen >= numVertices) {
        cout << "Error: vértice inválido. Debe estar entre 0 y " << numVertices - 1 << "\n";
        return 1;
    }

    // Ejecutamos Bellman–Ford
    vector<int> resultado = bellmanFord(numVertices, aristas, origen);
    // Mostramos resultado
    if (resultado.size() == 1 && resultado[0] == -1) {
        cout << "Se detectó un ciclo negativo en el grafo." << '\n';
        return 0;
    }

    cout << "Distancias mínimas desde el vértice " << origen << ":\n";
    for (int i = 0; i < numVertices; ++i) {
        cout << "  a " << i << ": " << resultado[i] << '\n';
    }

    return 0;
}