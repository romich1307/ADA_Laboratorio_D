#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <climits>

using namespace std;

const int INFINITO = INT_MAX / 2;

// Función para convertir índice a letra
char indiceLetter(int i) {
    return 'A' + i;
}

// Función para generar grafo aleatorio
void generarGrafoAleatorio(vector<vector<int>>& matriz, int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> prob(0.0, 1.0);
    uniform_int_distribution<> peso(1, 50);
    
    // Inicializar matriz
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                matriz[i][j] = 0;  // Distancia a sí mismo es 0
            } else if (prob(gen) < 0.6) {  // 60% probabilidad de arista
                matriz[i][j] = peso(gen);
            } else {
                matriz[i][j] = INFINITO;
            }
        }
    }
}

// Mostrar matriz de distancias
void mostrarMatrizDistancias(const vector<vector<int>>& matriz, const string& titulo) {
    cout << "\n" << titulo << "\n";
    
    // Línea superior
    cout << "     ";
    for (int j = 0; j < (int)matriz.size(); j++) {
        cout << "------";
    }
    cout << "\n";
    
    // Encabezado con letras
    cout << "     ";
    for (int j = 0; j < (int)matriz.size(); j++) {
        cout << "   " << indiceLetter(j) << "  ";
    }
    cout << "\n";
    
    // Línea separadora
    cout << "     ";
    for (int j = 0; j < (int)matriz.size(); j++) {
        cout << "------";
    }
    cout << "\n";
    
    // Filas con datos
    for (int i = 0; i < (int)matriz.size(); i++) {
        cout << " " << indiceLetter(i) << " | ";
        for (int j = 0; j < (int)matriz.size(); j++) {
            if (matriz[i][j] == INFINITO) {
                cout << "  ∞   ";
            } else {
                cout << setw(4) << matriz[i][j] << "  ";
            }
        }
        cout << "\n";
    }
    
    // Línea inferior
    cout << "     ";
    for (int j = 0; j < (int)matriz.size(); j++) {
        cout << "------";
    }
    cout << "\n\n";
}

// Mostrar matriz de recorridos
void mostrarMatrizRecorridos(const vector<vector<int>>& recorridos, const string& titulo) {
    cout << titulo << "\n";
    
    // Línea superior
    cout << "     ";
    for (int j = 0; j < (int)recorridos.size(); j++) {
        cout << "------";
    }
    cout << "\n";
    
    // Encabezado con letras
    cout << "     ";
    for (int j = 0; j < (int)recorridos.size(); j++) {
        cout << "   " << indiceLetter(j) << "  ";
    }
    cout << "\n";
    
    // Línea separadora
    cout << "     ";
    for (int j = 0; j < (int)recorridos.size(); j++) {
        cout << "------";
    }
    cout << "\n";
    
    // Filas con datos
    for (int i = 0; i < (int)recorridos.size(); i++) {
        cout << " " << indiceLetter(i) << " | ";
        for (int j = 0; j < (int)recorridos.size(); j++) {
            if (recorridos[i][j] == -1) {
                cout << setw(4) << "-" << "  ";
            } else {
                cout << setw(4) << indiceLetter(recorridos[i][j]) << "  ";
            }
        }
        cout << "\n";
    }
    
    // Línea inferior
    cout << "     ";
    for (int j = 0; j < (int)recorridos.size(); j++) {
        cout << "------";
    }
    cout << "\n\n";
}

// Algoritmo Floyd-Warshall paso a paso
void floydWarshallPasoAPaso(vector<vector<int>>& dist, vector<vector<int>>& recorridos) {
    int n = dist.size();
    
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
    
    cout << "=== ESTADO INICIAL ===\n";
    mostrarMatrizDistancias(dist, "MATRIZ DE DISTANCIAS INICIAL:");
    mostrarMatrizRecorridos(recorridos, "MATRIZ DE RECORRIDOS INICIAL:");
    
    // Algoritmo principal - k es el vértice intermedio
    for (int k = 0; k < n; k++) {
        cout << "=== ITERACIÓN k=" << indiceLetter(k) << " (usando vértice " << indiceLetter(k) << " como intermedio) ===\n";
        
        bool hubo_cambios = false;
        vector<string> cambios;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // Si existe camino i->k y k->j, verificar si es mejor
                if (dist[i][k] != INFINITO && dist[k][j] != INFINITO) {
                    int nueva_distancia = dist[i][k] + dist[k][j];
                    
                    if (nueva_distancia < dist[i][j]) {
                        cout << "  Mejorando ruta de " << indiceLetter(i) << " a " << indiceLetter(j) << ": ";
                        cout << dist[i][j];
                        if (dist[i][j] == INFINITO) cout << "(∞)";
                        cout << " -> " << nueva_distancia;
                        cout << " (pasando por " << indiceLetter(k) << ")\n";
                        
                        dist[i][j] = nueva_distancia;
                        recorridos[i][j] = recorridos[i][k]; // Actualizar recorrido
                        hubo_cambios = true;
                    }
                }
            }
        }
        
        if (!hubo_cambios) {
            cout << "  No hay mejoras en esta iteración.\n";
        }
        
        cout << "\n";
        mostrarMatrizDistancias(dist, "MATRIZ DE DISTANCIAS después de k=" + string(1, indiceLetter(k)) + ":");
        mostrarMatrizRecorridos(recorridos, "MATRIZ DE RECORRIDOS después de k=" + string(1, indiceLetter(k)) + ":");
        
        cout << "========================================\n";
    }
}

// Función para mostrar un camino específico
void mostrarCamino(const vector<vector<int>>& recorridos, const vector<vector<int>>& dist, int origen, int destino) {
    if (dist[origen][destino] == INFINITO) {
        cout << "No hay camino de " << indiceLetter(origen) << " a " << indiceLetter(destino) << "\n";
        return;
    }
    
    cout << "Camino de " << indiceLetter(origen) << " a " << indiceLetter(destino) 
         << " (distancia: " << dist[origen][destino] << "): ";
    
    vector<int> camino;
    int actual = origen;
    camino.push_back(actual);
    
    while (actual != destino) {
        actual = recorridos[actual][destino];
        if (actual == -1) break;
        camino.push_back(actual);
    }
    
    for (size_t i = 0; i < camino.size(); i++) {
        cout << indiceLetter(camino[i]);
        if (i < camino.size() - 1) cout << " → ";
    }
    cout << "\n";
}

int main() {
    cout << "=== ALGORITMO FLOYD-WARSHALL PASO A PASO ===\n";
    cout << "Grafo dirigido con 6 vértices\n\n";
    
    const int n = 6;
    vector<vector<int>> original(n, vector<int>(n));
    vector<vector<int>> distancias(n, vector<int>(n));
    vector<vector<int>> recorridos(n, vector<int>(n));
    
    // Generar grafo aleatorio
    generarGrafoAleatorio(original, n);
    
    // Copiar matriz original a matriz de distancias
    distancias = original;
    
    // Mostrar matriz original
    mostrarMatrizDistancias(original, "MATRIZ ORIGINAL (ADYACENCIA):");
    
    // Aplicar Floyd-Warshall paso a paso
    cout << "\n*** INICIANDO ALGORITMO FLOYD-WARSHALL ***\n";
    floydWarshallPasoAPaso(distancias, recorridos);
    
    // Mostrar resultado final
    cout << "\n=== RESULTADO FINAL ===\n";
    mostrarMatrizDistancias(distancias, "MATRIZ DE DISTANCIAS MÍNIMAS FINAL:");
    mostrarMatrizRecorridos(recorridos, "MATRIZ DE RECORRIDOS FINAL:");
    
    // Mostrar algunos ejemplos de caminos reconstruidos
    cout << "=== EJEMPLOS DE CAMINOS RECONSTRUIDOS ===\n";
    mostrarCamino(recorridos, distancias, 0, 3);
    mostrarCamino(recorridos, distancias, 1, 4);
    mostrarCamino(recorridos, distancias, 2, 5);
    mostrarCamino(recorridos, distancias, 5, 1);
    
    return 0;
}