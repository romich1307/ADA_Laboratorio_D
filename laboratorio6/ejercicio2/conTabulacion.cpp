// Programa 2: Camino de costo minimo - Tabulacion (Programacion dinamica)
// Descripcion en primera persona:
// - Yo implemento la version por tabulacion (bottom-up) para encontrar
//   el camino de menor costo desde (0,0) hasta (m-1,n-1) en una matriz de costos.
// - Uso una tabla dp[m][n] donde dp[i][j] es el costo minimo para llegar a (i,j).

#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

int costoMinimoTabulacion(vector<vector<int>>& costo) {
    int filas = costo.size();
    int columnas = costo[0].size();
    
    vector<vector<int>> tabla(filas, vector<int>(columnas, 0));
    
    // Inicializo la celda base
    tabla[0][0] = costo[0][0];

    // Relleno la primera fila
    for (int j = 1; j < columnas; j++) {
        tabla[0][j] = tabla[0][j - 1] + costo[0][j];
    }

    // Relleno la primera columna
    for (int i = 1; i < filas; i++) {
        tabla[i][0] = tabla[i - 1][0] + costo[i][0];
    }

    // Relleno el resto de la tabla
    for (int i = 1; i < filas; i++) {
        for (int j = 1; j < columnas; j++) {
            tabla[i][j] = costo[i][j] + min({tabla[i - 1][j],
                            tabla[i][j - 1], tabla[i - 1][j - 1]});
        }
    }

    return tabla[filas - 1][columnas - 1];
}

int main() {
    // Ejemplo de prueba: matriz de costos
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

    cout << "Programa 2: Camino minimo - Tabulacion" << endl;
    auto inicio = high_resolution_clock::now();
    int resultado = costoMinimoTabulacion(costo);
    auto fin = high_resolution_clock::now();

    cout << "Costo minimo (tabulacion): " << resultado << endl;
    auto dur = duration_cast<microseconds>(fin - inicio).count();
    cout << "Duracion: " << dur << " microsegundos" << endl;
    return 0;
}