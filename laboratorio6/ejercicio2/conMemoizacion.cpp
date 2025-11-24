#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Programa 2: Camino de costo minimo - Memoizacion (top-down)
// Descripcion en primera persona:
// - Yo implemento la version recursiva con memoizacion para encontrar
//   el costo minimo desde (0,0) hasta (m-1,n-1) en una matriz de costos.
// - Uso una tabla memo[m][n] inicializada a -1 para almacenar subresultados.

// Funcion recursiva con memo
int costoMinimoMemoRec(vector<vector<int>>& costo, int i, int j, vector<vector<int>> &memo) {
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

    // Si ya calcule el resultado, lo retorno
    if (memo[i][j] != -1) {
        return memo[i][j];
    }

    // Calculo recursivo: derecha, abajo y diagonal
    int derecha = costoMinimoMemoRec(costo, i, j + 1, memo);
    int abajo = costoMinimoMemoRec(costo, i + 1, j, memo);
    int diag = costoMinimoMemoRec(costo, i + 1, j + 1, memo);
    int mejor = min({derecha, abajo, diag});

    // Evito overflow si todas las opciones son INT_MAX
    if (mejor == INT_MAX) memo[i][j] = INT_MAX;
    else memo[i][j] = costo[i][j] + mejor;
    return memo[i][j];
}

int costoMinimoMemo(vector<vector<int>>& costo) {
    int filas = costo.size();
    int columnas = costo[0].size();
    vector<vector<int>> memo(filas, vector<int>(columnas, -1));
    return costoMinimoMemoRec(costo, 0, 0, memo);
}

int main() {
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

    cout << "Programa 2: Camino minimo - Memoizacion" << endl;
    auto inicio = high_resolution_clock::now();
    int resultado = costoMinimoMemo(costo);
    auto fin = high_resolution_clock::now();

    cout << "Costo minimo (memoizacion): " << resultado << endl;
    auto dur = duration_cast<microseconds>(fin - inicio).count();
    cout << "Duracion: " << dur << " microsegundos" << endl;
    return 0;
}