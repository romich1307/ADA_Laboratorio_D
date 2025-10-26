#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <climits>
#include <iomanip>
#include <algorithm>

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
                cout << setw(4) << "∞" << "  ";
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
            cout << "Iteración k=" << indiceLetter(k) << " - Se encontraron mejoras\n";
        }
    }
    
    // Mostrar resultado final para grafos pequeños
    if (n <= 8) {
        cout << "\n=== FLOYD-WARSHALL: RESULTADO FINAL ===\n";
        mostrarMatrizSimple(dist, "MATRIZ DE DISTANCIAS MÍNIMAS FINAL:");
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
    
    // TODO: IMPLEMENTAR ALGORITMO DIJKSTRA AQUÍ
    // - Usar cola de prioridad (priority_queue)
    // - Inicializar dist[origen] = 0
    // - Mientras cola no esté vacía:
    //   - Extraer vértice con menor distancia
    //   - Relajar aristas adyacentes
    // 
    // NOTA: Este es un algoritmo de camino más corto desde un origen
    // Complejidad: O((V + E) log V)
    
    dist[origen] = 0;
    // IMPLEMENTACIÓN PENDIENTE...
    
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
    
    // TODO: IMPLEMENTAR ALGORITMO BELLMAN-FORD AQUÍ
    // - Inicializar dist[origen] = 0
    // - Repetir V-1 veces:
    //   - Para cada arista (u,v) con peso w:
    //     - Si dist[u] + w < dist[v]: dist[v] = dist[u] + w
    // - Verificar ciclos negativos (opcional)
    //
    // NOTA: Este algoritmo puede manejar pesos negativos
    // Complejidad: O(V * E)
    
    dist[origen] = 0;
    // IMPLEMENTACIÓN PENDIENTE...
    
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

void analizarComplejidades() {
    cout << "\n=== ANÁLISIS TEÓRICO DE COMPLEJIDADES ===\n\n";
    
    cout << "FLOYD-WARSHALL:\n";
    cout << "  • Complejidad: O(V³)\n";
    cout << "  • Ventajas: Encuentra caminos mínimos entre TODOS los pares\n";
    cout << "  • Desventajas: Muy lento para grafos grandes\n";
    cout << "  • Uso: Grafos pequeños/medianos, matrices de distancias\n\n";
    
    cout << "DIJKSTRA:\n";
    cout << "  • Complejidad: O((V + E) log V)\n";
    cout << "  • Ventajas: Muy eficiente, desde un origen a todos\n";
    cout << "  • Desventajas: NO maneja pesos negativos\n";
    cout << "  • Uso: Caminos más cortos con pesos positivos\n\n";
    
    cout << "BELLMAN-FORD:\n";
    cout << "  • Complejidad: O(V * E)\n";
    cout << "  • Ventajas: Maneja pesos negativos, detecta ciclos negativos\n";
    cout << "  • Desventajas: Más lento que Dijkstra\n";
    cout << "  • Uso: Grafos con pesos negativos\n\n";
}

void mostrarRecomendaciones() {
    cout << "=== RECOMENDACIONES DE USO ===\n\n";
    cout << " FLOYD-WARSHALL:\n";
    cout << "   ✓ Usar cuando necesites TODAS las distancias\n";
    cout << "   ✓ Grafos pequeños (< 500 vértices)\n";
    cout << "   ✓ Matriz de adyacencia disponible\n\n";
    
    cout << " DIJKSTRA:\n";
    cout << "   ✓ Caminos desde UN origen específico\n";
    cout << "   ✓ Pesos NO negativos\n";
    cout << "   ✓ Grafos grandes y eficiencia importante\n\n";
    
    cout << " BELLMAN-FORD:\n";
    cout << "   ✓ Cuando hay pesos negativos\n";
    cout << "   ✓ Detección de ciclos negativos necesaria\n";
    cout << "   ✓ Grafos no muy densos\n\n";
}

// ============================================================================
// FUNCIÓN PRINCIPAL
// ============================================================================

int main() {
    cout << "=== COMPARACIÓN DE ALGORITMOS DE CAMINOS MÍNIMOS ===\n";
    cout << "Floyd-Warshall vs Dijkstra vs Bellman-Ford\n\n";
    
    vector<int> tamaños = {10, 25, 50, 100, 200}; // Tamaños de grafos a probar
    
    cout << "Probando diferentes tamaños de grafos...\n\n";
    cout << setw(8) << "Vértices" 
         << setw(15) << "Floyd-W (μs)" 
         << setw(15) << "Dijkstra (μs)" 
         << setw(15) << "Bellman-F (μs)" << "\n";
    cout << string(60, '-') << "\n";
    
    for (int n : tamaños) {
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
    
    // Análisis teórico
    analizarComplejidades();
    mostrarRecomendaciones();
    
    cout << "\n=== NOTAS IMPORTANTES ===\n";
    cout << "• Los tiempos pueden variar según el hardware\n";
    cout << "• Floyd-Warshall calcula TODOS los caminos\n";
    cout << "• Dijkstra y Bellman-Ford calculan desde UN origen\n";
    cout << "• Para comparación justa, ejecutar Dijkstra V veces\n";
    cout << "• Densidad del grafo afecta significativamente el rendimiento\n\n";
    
    return 0;
}