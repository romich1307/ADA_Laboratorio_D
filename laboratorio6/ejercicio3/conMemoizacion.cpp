// Programa 3: Mochila 0/1
// Implementación con memoización (top-down) usando recursion + tabla de memo
// Los resultados son equivalentes a la tabulación, pero la implementación es recursiva.
// Complejidad: O(n * W) tiempo en el peor caso, O(n * W) espacio para la memo.

#include <iostream>
#include <vector>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

// knapsackMemoUtil: función recursiva que usa memo[i][W] para guardar resultados
// Parámetros:
//  - W: capacidad restante
//  - w: pesos
//  - v: valores
//  - i: número de objetos considerados (primeros i)
//  - memo: tabla (i x W) con resultados ya calculados o -1 si no calculado
int knapsackMemoUtil(int W, vector<int>& w, vector<int>& v, int i, vector<vector<int>>& memo) {
	if (i == 0 || W == 0) return 0; // casos base
	if (memo[i][W] != -1) return memo[i][W];

	if (w[i - 1] <= W) {
		// Opción de tomar o no tomar el objeto i-1
		int take = v[i - 1] + knapsackMemoUtil(W - w[i - 1], w, v, i - 1, memo);
		int notake = knapsackMemoUtil(W, w, v, i - 1, memo);
		memo[i][W] = max(take, notake);
	} else {
		// No cabe, se omite el objeto
		memo[i][W] = knapsackMemoUtil(W, w, v, i - 1, memo);
	}

	return memo[i][W];
}

// knapsackMemo: wrapper que inicializa la tabla de memoización
int knapsackMemo(int W, vector<int>& w, vector<int>& v, int n) {
	vector<vector<int>> memo(n + 1, vector<int>(W + 1, -1));
	return knapsackMemoUtil(W, w, v, n, memo);
}

int main() {
	// Datos de ejemplo ampliados para comparar tiempos (n = 30)
	vector<int> valor = {10,20,30,25,40,35,45,50,8,38,60,5,70,65,18,22,14,55,33,48,27,19,44,58,61,72,13,17,29,36};
	vector<int> peso  = {3,4,6,5,8,7,9,11,2,10,12,1,14,13,4,6,3,9,5,10,7,4,8,11,12,15,2,3,6,7};
	int W = 150; // capacidad de la mochila
	int n = static_cast<int>(valor.size());

	// Medición de tiempo
	auto inicio = high_resolution_clock::now();
	int resultado = knapsackMemo(W, peso, valor, n);
	auto fin = high_resolution_clock::now();

	cout << "Programa 3: Mochila 0/1 - Memoizacion" << endl;
	cout << "Valor maximo: " << resultado << endl;
	auto duracion = duration_cast<microseconds>(fin - inicio).count();
	cout << "La duracion del algoritmo con memoizacion es: " << duracion << " microsegundos" << endl;

	return 0;
}
