// Programa 4: Mochila fraccional - Tabulacion (ordenacion + prefijos)
// Descripcion en primera persona:
// - Yo implemento la mochila fraccional usando una etapa de ordenacion por
//   ratio valor/peso y luego uso la lista ordenada para llenar la capacidad.
// - En este archivo lo llamo "tabulacion" porque yo precomputo orden y
//   prefijos para facilitar el calculo del valor total; la complejidad es
//   O(n log n) por la ordenacion.

#include <iostream>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

// Implementacion de la mochila 0/1 con tabulacion (2D)
struct ItemF {
	int peso;
	int valor;
	double ratio;
};

double mochila_fraccional_tabulacion(int W, vector<int>& peso, vector<int>& valor) {
	int n = (int)peso.size();
	vector<ItemF> items(n);
	for (int i = 0; i < n; ++i) {
		items[i].peso = peso[i];
		items[i].valor = valor[i];
		items[i].ratio = (double)valor[i] / (double)peso[i];
	}
	// ordeno por ratio descendente
	sort(items.begin(), items.end(), [](const ItemF& a, const ItemF& b){
		return a.ratio > b.ratio;
	});

	// uso la lista ordenada para llenar la mochila; esto es esencialmente
	// la forma optima para la mochila fraccional
	double valor_total = 0.0;
	int capacidad = W;
	for (const auto& it : items) {
		if (capacidad == 0) break;
		if (it.peso <= capacidad) {
			valor_total += it.valor;
			capacidad -= it.peso;
		} else {
			double fraccion = (double)capacidad / (double)it.peso;
			valor_total += it.valor * fraccion;
			capacidad = 0;
		}
	}
	return valor_total;
}

int main() {
vector<int> valor = {10,20,30,25,40,35,45,50,8,38,60,5,70,65,18,22,14,55,33
                    ,48,27,19,44,58,61,72,13,17,29,36};
vector<int> peso  = {3,4,6,5,8,7,9,11,2,10,12,1,14,13,4,6,3,9,5,10,7,4,8,11,
                    12,15,2,3,6,7};
	int W = 150;

	cout << "Programa 4: Mochila fraccional - Tabulacion" << endl;

	auto inicio = high_resolution_clock::now();
	double resultado = mochila_fraccional_tabulacion(W, peso, valor);
	auto fin = high_resolution_clock::now();

	cout << "Valor maximo (fraccional tabulacion): " << resultado << endl;
	auto duracion = duration_cast<microseconds>(fin - inicio).count();
	cout << "Duracion: " << duracion << " microsegundos" << endl;

	return 0;
}

