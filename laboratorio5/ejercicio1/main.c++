#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

// Función para construir la lista de adyacencia
vector<vector<vector<int>>> construirAdyacencia(vector<vector<int>> &aristas, int numVertices) {
    // listaAdyacencia[u] = lista de {v, peso}
    vector<vector<vector<int>>> listaAdyacencia(numVertices); 

    for (const auto &arista : aristas) {
        int u = arista[0];
        int v = arista[1];
        int peso = arista[2];
        listaAdyacencia[u].push_back({v, peso});
        listaAdyacencia[v].push_back({u, peso}); // Grafo no dirigido
    }
    
    return listaAdyacencia;
}

// Retorna las distancias más cortas desde origen a todos los demás vértices
vector<int> dijkstra(int numVertices, vector<vector<int>> &aristas, int origen){
    
    // Crear lista de adyacencia
    vector<vector<vector<int>>> listaAdyacencia = construirAdyacencia(aristas, numVertices);

    // Crear una cola de prioridad para almacenar los vértices
    // que están siendo procesados
    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> colaPrioridad;

    // Crear un vector para distancias e inicializar todas
    // las distancias como infinito
    vector<int> distancia(numVertices, INT_MAX);

    // Insertar el vértice origen en la cola de prioridad
    // e inicializar su distancia como 0
    colaPrioridad.push({0, origen});
    distancia[origen] = 0;

    // Iterar hasta que la cola de prioridad esté vacía
    // (o todas las distancias estén finalizadas) 
    while (!colaPrioridad.empty()){
        
        // El primer vértice en el par es el vértice con distancia mínima,
        // extraerlo de la cola de prioridad
        int u = colaPrioridad.top()[1];
        colaPrioridad.pop();

        // Obtener todos los adyacentes de u
        for (auto vecino : listaAdyacencia[u]){
            
            // Obtener la etiqueta del vértice y el peso del
            // adyacente actual de u
            int v = vecino[0];
            int peso = vecino[1];

            // Si hay un camino más corto a v a través de u
            if (distancia[v] > distancia[u] + peso)
            {
                // Actualizar la distancia de v
                distancia[v] = distancia[u] + peso;
                colaPrioridad.push({distancia[v], v});
            }
        }
    }

    return distancia;
}

// Programa principal para probar el algoritmo de Dijkstra
int main(){
    // Inicializar semilla para números aleatorios
    srand(time(0));
    
    int numVertices = 8;  // 8 vértices (0 a 7)
    int origen;           // Vértice origen (elegido por el usuario)

    // Lista de aristas: {u, v, peso}
    // Grafo con 15 aristas - estructura de conexiones
    vector<vector<int>> aristas = {
        {0, 1, 0},   // Vértice 0 -> 1
        {0, 2, 0},   // Vértice 0 -> 2
        {1, 2, 0},   // Vértice 1 -> 2
        {1, 3, 0},   // Vértice 1 -> 3
        {2, 3, 0},   // Vértice 2 -> 3
        {2, 4, 0},   // Vértice 2 -> 4
        {3, 4, 0},   // Vértice 3 -> 4
        {3, 5, 0},   // Vértice 3 -> 5
        {4, 5, 0},   // Vértice 4 -> 5
        {4, 6, 0},   // Vértice 4 -> 6
        {5, 6, 0},   // Vértice 5 -> 6
        {5, 7, 0},   // Vértice 5 -> 7
        {6, 7, 0},   // Vértice 6 -> 7
        {0, 7, 0},   // Vértice 0 -> 7
        {1, 4, 0}    // Vértice 1 -> 4
    };

    // Generar pesos aleatorios entre 1 y 10 para cada arista
    for (auto& arista : aristas) {
        arista[2] = 1 + rand() % 10;  // Peso aleatorio entre 1 y 10
    }

    cout << "=== ALGORITMO DE DIJKSTRA ===" << endl;
    cout << "Grafo: " << numVertices << " vértices, " << aristas.size() << " aristas" << endl;
    cout << "Pesos generados aleatoriamente entre 1 y 10" << endl;
    cout << endl;

    // Mostrar las aristas del grafo
    cout << "Lista de aristas (u -> v: peso):" << endl;
    for (const auto& arista : aristas) {
        cout << "  " << arista[0] << " -> " << arista[1] << ": " << arista[2] << endl;
    }
    cout << endl;

    // Pedir al usuario que elija el vértice origen
    cout << "Ingrese el vértice origen (0-" << numVertices-1 << "): ";
    cin >> origen;

    // Validar entrada
    if (origen < 0 || origen >= numVertices) {
        cout << "Error: Vértice inválido. Debe estar entre 0 y " << numVertices-1 << endl;
        return 1;
    }

    cout << endl;

    // Ejecutar algoritmo de Dijkstra
    vector<int> resultado = dijkstra(numVertices, aristas, origen);

    // Mostrar resultados
    cout << "Distancias más cortas desde el vértice " << origen << ":" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Vértice\t\tDistancia" << endl;
    cout << "-----------------------------------" << endl;
    
    for (int i = 0; i < numVertices; i++) {
        cout << "   " << i << "\t\t";
        if (resultado[i] == INT_MAX)
            cout << "INF (inalcanzable)" << endl;
        else
            cout << resultado[i] << endl;
    }
    cout << "-----------------------------------" << endl;
 
    return 0;
}
