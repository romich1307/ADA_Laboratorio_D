// Programa 4: Mochila fraccional - Memoizacion (top-down)
// Descripcion en primera persona:
// - Yo implemento una version recursiva para la mochila fraccional y uso memoizacion
//   sobre el par (indice, capacidad_restante) para evitar recalcular subproblemas
// - En la practica la solucion greedy es directa, pero muestro esta variante para
//   comparar estilos (memoizacion aplicada aunque no sea necesaria para el caso fraccional)

#include <iostream>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

double knap_frac_rec(int idx, int remW, vector<int>& peso, vector<int>& valor, vector<unordered_map<int,double>>& memo) {
	if (idx >= (int)peso.size() || remW == 0) return 0.0;
	auto it = memo[idx].find(remW);
	if (it != memo[idx].end()) return it->second;
	// si el objeto cabe entero, opcion de tomarlo completo y seguir
	double tomar = 0.0;
	if (peso[idx] <= remW) {
		tomar = valor[idx] + knap_frac_rec(idx+1, remW - peso[idx], peso, valor, memo);
	} else {
		// tomar fraccion y terminar (no queda capacidad)
		double fr = (double)remW / (double)peso[idx];
		tomar = valor[idx] * fr;
		memo[idx][remW] = tomar;
		return tomar;
	}
	// opcion de no tomar y pasar al siguiente
	double no_tomar = knap_frac_rec(idx+1, remW, peso, valor, memo);
	double ans = max(tomar, no_tomar);
	memo[idx][remW] = ans;
	return ans;
}

double mochila_fraccional_memo(int W, vector<int>& peso, vector<int>& valor) {
	int n = (int)peso.size();
	vector<unordered_map<int,double>> memo(n);
	return knap_frac_rec(0, W, peso, valor, memo);
}

int main() {
	// Conjunto de prueba: 30 items
    vector<int> valor = {100,60, 110, 120}; // valores de los objetos
    vector<int> peso = {10,30, 20, 50};    // pesos de los objetos
	int W = 150;

	cout << "Programa 4: Mochila fraccional - Memoizacion (top-down)" << endl;

	auto inicio = high_resolution_clock::now();
	double resultado = mochila_fraccional_memo(W, peso, valor);
	auto fin = high_resolution_clock::now();

	cout << "Valor maximo (fraccional memo): " << resultado << endl;
	auto duracion = duration_cast<microseconds>(fin - inicio).count();
	cout << "Duracion: " << duracion << " microsegundos" << endl;

	return 0;
}

