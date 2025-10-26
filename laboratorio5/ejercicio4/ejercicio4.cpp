#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <climits>
#include <iomanip>
#include <algorithm>
#include <queue>

using namespace std;
using namespace chrono;

const int INFINITO = INT_MAX / 2;

// ============================================================================
// GENERACIÓN DE GRAFOS ALEATORIOS
// ============================================================================

class Grafo {
public:
    int vertices;
    vector<vector<int>> matriz;
    vector<vector<pair<int, int>>> lista_adyacencia; // {destino, peso}
    
    Grafo(int n) : vertices(n) {
        matriz.resize(n, vector<int>(n, INFINITO));
        lista_adyacencia.resize(n);
        
        // Diagonal principal en 0
        for (int i = 0; i < n; i++) {
            matriz[i][i] = 0;
        }
    }
    
    void agregarArista(int origen, int destino, int peso) {
        matriz[origen][destino] = peso;
        lista_adyacencia[origen].push_back({destino, peso});
    }
    
    void generarAleatorio(double densidad = 0.3) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> prob(0.0, 1.0);
        uniform_int_distribution<> peso(1, 100);
        
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (i != j && prob(gen) < densidad) {
                    agregarArista(i, j, peso(gen));
                }
            }
        }
    }
};

// ============================================================================
// ALGORITMO FLOYD-WARSHALL (del ejercicio2 - versión completa)
// ============================================================================

// Función para convertir índice a letra
char indiceLetter(int i) {
    return 'A' + i;
}

// Mostrar matriz de distancias (versión simple para comparación)
void mostrarMatrizSimple(const vector<vector<int>>& matriz, const string& titulo, int maxSize = 8) {
    if (matriz.size() > maxSize) {
        cout << titulo << " (matriz muy grande, no se muestra)\n";
        return;
    }
    
    cout << "\n" << titulo << "\n";
    cout << "     ";
    for (int j = 0; j < (int)matriz.size(); j++) {
        cout << setw(6) << indiceLetter(j);
    }
    cout << "\n";
    
    for (int i = 0; i < (int)matriz.size(); i++) {
        cout << " " << indiceLetter(i) << " | ";
        for (int j = 0; j < (int)matriz.size(); j++) {
            if (matriz[i][j] == INFINITO) {
                cout << setw(4) << "INF" << "  ";
            } else {
                cout << setw(4) << matriz[i][j] << "  ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

pair<long long, vector<vector<int>>> floydWarshall(Grafo& g) {
    auto inicio = high_resolution_clock::now();
    
    vector<vector<int>> dist = g.matriz;
    vector<vector<int>> recorridos(g.vertices, vector<int>(g.vertices));
    int n = g.vertices;
    
    // Inicializar matriz de recorridos
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                recorridos[i][j] = -1; // No hay recorrido hacia sí mismo
            } else if (dist[i][j] != INFINITO) {
                recorridos[i][j] = j; // Camino directo
            } else {
                recorridos[i][j] = -1; // No hay camino
            }
        }
    }
    
    // Mostrar estado inicial para grafos pequeños
    if (n <= 8) {
    cout << "\n=== FLOYD-WARSHALL: ESTADO INICIAL ===\n";
    mostrarMatrizSimple(dist, "MATRIZ DE DISTANCIAS INICIAL:");
    }
    
    // Algoritmo Floyd-Warshall principal
    for (int k = 0; k < n; k++) {
        bool hubo_cambios = false;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // Si existe camino i->k y k->j, verificar si es mejor
                if (dist[i][k] != INFINITO && dist[k][j] != INFINITO && 
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    
                    dist[i][j] = dist[i][k] + dist[k][j];
                    recorridos[i][j] = recorridos[i][k]; // Actualizar recorrido
                    hubo_cambios = true;
                }
            }
        }
        
        // Mostrar progreso para grafos pequeños
        if (n <= 8 && hubo_cambios) {
            cout << "Iteracion k=" << indiceLetter(k) << " - Se encontraron mejoras\n";
        }
    }
    
    // Mostrar resultado final para grafos pequeños
    if (n <= 8) {
    cout << "\n=== FLOYD-WARSHALL: RESULTADO FINAL ===\n";
    mostrarMatrizSimple(dist, "MATRIZ DE DISTANCIAS MINIMAS FINAL:");
    }
    
    auto fin = high_resolution_clock::now();
    long long tiempo = duration_cast<microseconds>(fin - inicio).count();
    
    return {tiempo, dist};
}

// ============================================================================
// ALGORITMO DIJKSTRA Paola
// ============================================================================

pair<long long, vector<int>> dijkstra(Grafo& g, int origen) {
    auto inicio = high_resolution_clock::now();
    
    vector<int> dist(g.vertices, INFINITO);
    // Cola de prioridad: (distancia, nodo). Menor distancia primero
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

    dist[origen] = 0;
    pq.push({0, origen});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue; // Entrada obsoleta

        for (const auto& [v, peso] : g.lista_adyacencia[u]) {
            if (dist[u] != INFINITO && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                pq.push({dist[v], v});
            }
        }
    }
    
    auto fin = high_resolution_clock::now();
    long long tiempo = duration_cast<microseconds>(fin - inicio).count();
    
    return {tiempo, dist};
}

// ============================================================================
// ALGORITMO BELLMAN-FORD paola
// ============================================================================

pair<long long, vector<int>> bellmanFord(Grafo& g, int origen) {
    auto inicio = high_resolution_clock::now();
    
    vector<int> dist(g.vertices, INFINITO);
    
    dist[origen] = 0;
    int V = g.vertices;

    // Relajar todas las aristas V-1 veces
    for (int i = 0; i < V - 1; ++i) {
        bool cambio = false;
        for (int u = 0; u < V; ++u) {
            if (dist[u] == INFINITO) continue;
            for (const auto& [v, w] : g.lista_adyacencia[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    cambio = true;
                }
            }
        }
        if (!cambio) break; // Optimización: terminar si no hubo cambios
    }

    // Detección de ciclo negativo (opcional; no debería ocurrir con pesos positivos)
    for (int u = 0; u < V; ++u) {
        if (dist[u] == INFINITO) continue;
        for (const auto& [v, w] : g.lista_adyacencia[u]) {
            if (dist[u] + w < dist[v]) {
                // En presencia de ciclo negativo, dejamos las distancias actuales
                // (para la comparación de tiempos no afecta) y salimos.
                u = V; // romper ambos bucles
                break;
            }
        }
    }
    
    auto fin = high_resolution_clock::now();
    long long tiempo = duration_cast<microseconds>(fin - inicio).count();
    
    return {tiempo, dist};
}

// ============================================================================
// FUNCIONES DE ANÁLISIS Y COMPARACIÓN
// ============================================================================

void mostrarComparacion(int vertices, long long tiempo_floyd, 
                       long long tiempo_dijkstra, long long tiempo_bellman) {
    cout << setw(8) << vertices 
         << setw(15) << tiempo_floyd 
         << setw(15) << tiempo_dijkstra 
         << setw(15) << tiempo_bellman << "\n";
}


// ============================================================================
// FUNCIÓN PRINCIPAL
// ============================================================================

int main() {
    cout << "=== COMPARACION DE ALGORITMOS DE CAMINOS MINIMOS ===\n";
    cout << "Floyd-Warshall vs Dijkstra vs Bellman-Ford\n\n";
    
    vector<int> tamanos = {10, 25, 50, 100, 200}; // Tamanos de grafos a probar
    
    cout << "Probando diferentes tamanos de grafos...\n\n";
    cout << setw(8) << "Vertices" 
         << setw(15) << "Floyd-W (us)" 
         << setw(15) << "Dijkstra (us)" 
         << setw(15) << "Bellman-F (us)" << "\n";
    cout << string(60, '-') << "\n";
    
    for (int n : tamanos) {
        Grafo g(n);
        g.generarAleatorio(0.3); // 30% de densidad
        
        // Medir Floyd-Warshall
        auto [tiempo_floyd, resultado_floyd] = floydWarshall(g);
        
        // Medir Dijkstra (desde vértice 0)
        auto [tiempo_dijkstra, resultado_dijkstra] = dijkstra(g, 0);
        
        // Medir Bellman-Ford (desde vértice 0)
        auto [tiempo_bellman, resultado_bellman] = bellmanFord(g, 0);
        
        mostrarComparacion(n, tiempo_floyd, tiempo_dijkstra, tiempo_bellman);
    }


    return 0;
}