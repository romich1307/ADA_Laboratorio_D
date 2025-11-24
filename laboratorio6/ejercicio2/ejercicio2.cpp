// Programa 2: Camino de costo minimo - Recursivo (sin memo)
// Descripcion en primera persona:
// - Yo implemento la version recursiva que explora todas las rutas posibles
//   para encontrar el camino de menor costo desde (0,0) hasta (m-1,n-1).
// - Esta version es exponencial en tiempo y se incluye con fines didacticos
//   para comparar con las versiones con memoizacion y tabulacion.

#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Funcion recursiva que devuelve el costo minimo desde (i,j) hasta el destino
int costoMinimoRecursivoRec(vector<vector<int>>& costo, int i, int j) {
    int filas = costo.size();
    int columnas = costo[0].size();
  
    // Si indices fuera de rango, devuelvo un valor grande
    if (i >= filas || j >= columnas) {
        return INT_MAX;
    }

    // Caso base: celda destino
    if (i == filas - 1 && j == columnas - 1) {
        return costo[i][j];
    }

    // Calculo recursivo de todas las direcciones posibles
    int derecha = costoMinimoRecursivoRec(costo, i, j + 1);
    int abajo = costoMinimoRecursivoRec(costo, i + 1, j);
    int diag = costoMinimoRecursivoRec(costo, i + 1, j + 1);
    int mejor = min({derecha, abajo, diag});
    if (mejor == INT_MAX) return INT_MAX;
    return costo[i][j] + mejor;
}

int costoMinimoRecursivo(vector<vector<int>>& costo) {
    return costoMinimoRecursivoRec(costo, 0, 0);
}

int main() {
    // Matriz de prueba 10x10
    vector<vector<int>> costo = {
        { 7, 2, 9, 10, 4, 12, 5, 8, 3, 6 },
        { 3, 14, 6, 11, 9, 2, 7, 13, 8, 5 },
        { 10, 4, 12, 5, 8, 3, 9, 6, 14, 7 },
        { 2, 9, 7, 13, 11, 4, 10, 5, 12, 8 },
        { 6, 3, 8, 2, 14, 9, 1, 11, 7, 13 },
        { 5, 12, 4, 9, 6, 10, 8, 2, 15, 3 },
        { 11, 7, 3, 14, 5, 6, 12, 9, 4, 10 },
        { 8, 1, 13, 7, 2, 15, 6, 14, 5, 9 },
        { 4, 10, 5, 3, 12, 8, 11, 7, 2, 14 },
        { 9, 6, 2, 12, 13, 1, 14, 4, 10, 11 }
    };

    cout << "Programa 2: Camino minimo - Recursivo (sin optimizacion)" << endl;
    auto inicio = high_resolution_clock::now();
    int resultado = costoMinimoRecursivo(costo);
    auto fin = high_resolution_clock::now();

    cout << "Costo minimo (recursivo): " << resultado << endl;
    auto dur = duration_cast<microseconds>(fin - inicio).count();
    cout << "Duracion: " << dur << " microsegundos" << endl;
    return 0;
}