// Programa 3: Mochila 0/1
// Implementación con programación dinámica - tabulación (bottom-up)
// Problema: dado un conjunto de n objetos con pesos w[i] y valores v[i],
// seleccionar un subconjunto con peso total ≤ W que maximice el valor total.
// Complejidad: O(n * W) tiempo, O(n * W) espacio (tabla dp).

#include <iostream>
#include <vector>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

// knapsack: versión con tabulación.
// Entradas:
//  - W: capacidad máxima de la mochila
//  - w: vector de pesos (tamaño n)
//  - v: vector de valores (tamaño n)
//  - n: número de objetos
// Salida: valor máximo alcanzable sin superar W
int knapsack(int W, vector<int>& w, vector<int>& v, int n) {
    // dp[i][j] = valor máximo usando los primeros i objetos con capacidad j
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= W; j++) {
            // Si el peso del objeto i-1 cabe en la capacidad j,
            // posibilidad de tomarlo o no tomarlo
            if (w[i - 1] <= j)
                dp[i][j] = max(v[i - 1] + dp[i - 1][j - w[i - 1]], dp[i - 1][j]);
            else
                // No cabe: heredamos el mejor sin el objeto i
                dp[i][j] = dp[i - 1][j];
        }
    }

    return dp[n][W];
}

int main() {
    // Datos de ejemplo ampliados para comparar tiempos (n = 30)
    vector<int> valor = {10,20,30,25,40,35,45,50,8,38,60,5,70,65,18,22,14,55,33,48,27,19,44,58,61,72,13,17,29,36};
    vector<int> peso  = {3,4,6,5,8,7,9,11,2,10,12,1,14,13,4,6,3,9,5,10,7,4,8,11,12,15,2,3,6,7};
    int W = 150;                         // capacidad de la mochila
    int n = static_cast<int>(valor.size());

    // Medición de tiempo (microsegundos) siguiendo estilo del ejercicio 1
    auto inicio = high_resolution_clock::now();
    int resultado = knapsack(W, peso, valor, n);
    auto fin = high_resolution_clock::now();

    cout << "Programa 3: Mochila 0/1 - Tabulacion" << endl;
    cout << "Valor maximo: " << resultado << endl;
    auto duracion = duration_cast<microseconds>(fin - inicio).count();
    cout << "La duracion del algoritmo con tabulacion es: " << duracion << " microsegundos" << endl;

    return 0;
}
