// Programa 3: Mochila 0/1
// Versión "normal" (bruto) recursiva sin memoización
// Un ladrón tiene una mochila con capacidad limitada de peso W, hay n objetos,
// cada uno con un peso w[i] y un valor v[i]. El objetivo es maximizar el valor total
// sin superar la capacidad W. Esta implementación explora todas las combinaciones
// posibles (complejidad exponencial O(2^n)) y se usa para comparar tiempos con
// versiones DP (memoización y tabulación).
#include <iostream>
#include <vector>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

// Versión "normal" (brute-force) recursiva sin memoización
// Explora todas las combinaciones: para cada objeto decide tomarlo o dejarlo.
// Tiene complejidad exponencial O(2^n) en tiempo y profundidad de recursión O(n).
int knapsackBrute(int W, vector<int>& w, vector<int>& v, int n) {
    // Caso base: no quedan objetos o la capacidad es 0
    if (n == 0 || W == 0) return 0;

    // Si el objeto actual (índice n-1) cabe en la capacidad restante,
    // podemos elegir entre tomarlo o no tomarlo.
    if (w[n - 1] <= W) {
        int take = v[n - 1] + knapsackBrute(W - w[n - 1], w, v, n - 1);
        int notake = knapsackBrute(W, w, v, n - 1);
        return max(take, notake);
    } else {
        // Si no cabe, sólo queda la opción de no tomarlo
        return knapsackBrute(W, w, v, n - 1);
    }
}

int main() {
    // Datos de ejemplo ampliados para comparar tiempos (n = 30)
    vector<int> valor = {10,20,30,25,40,35,45,50,8,38,60,5,70,65,18,22,14,55,33,48,27,19,44,58,61,72,13,17,29,36};
    vector<int> peso  = {3,4,6,5,8,7,9,11,2,10,12,1,14,13,4,6,3,9,5,10,7,4,8,11,12,15,2,3,6,7};
    int W = 150;                         // capacidad de la mochila
    int n = static_cast<int>(valor.size());

    auto inicio = high_resolution_clock::now();
    int resultado = knapsackBrute(W, peso, valor, n);
    auto fin = high_resolution_clock::now();

    // Mostrar encabezado con el nombre del programa y la variante ejecutada
    cout << "Programa 3: Mochila 0/1 - sin programacion Dinamica" << endl;
    cout << "Valor maximo: " << resultado << endl;
    auto duracion = duration_cast<microseconds>(fin - inicio).count();
    cout << "La duracion del algoritmo normal es: " << duracion << " microsegundos" << endl;

    return 0;
}
