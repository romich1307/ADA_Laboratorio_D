// Programa 4: Mochila fraccional - Sin optimizacion (seleccion repetida)
// Descripcion en primera persona:
// - Yo implemento una version sencilla (sin optimizacion) para la mochila fraccional.
// - En cada paso busco el objeto no tomado con mayor ratio valor/peso mediante
//   una busqueda lineal y lo tomo completo o en fraccion segun que quepa.
// - Esta version es O(n^2) por la busqueda repetida y sirve para comparar con
//   las versiones mas optimizadas.

#include <iostream>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

struct ItemSimple {
	int peso;
	int valor;
	bool tomado;
};

double mochila_fraccional_sin_opt(int W, vector<int>& peso, vector<int>& valor) {
	int n = (int)peso.size();
	vector<ItemSimple> items(n);
	for (int i = 0; i < n; ++i) {
		items[i].peso = peso[i];
		items[i].valor = valor[i];
		items[i].tomado = false;
	}

	double valor_total = 0.0;
	int capacidad = W;

	// Repetir hasta llenar la capacidad o no queden objetos
	while (capacidad > 0) {
		int idx_mejor = -1;
		double mejor_ratio = -1.0;
		// busqueda lineal del mejor ratio entre objetos no tomados
		for (int i = 0; i < n; ++i) {
			if (items[i].tomado) continue;
			double ratio = (double)items[i].valor / (double)items[i].peso;
			if (ratio > mejor_ratio) {
				mejor_ratio = ratio;
				idx_mejor = i;
			}
		}
		if (idx_mejor == -1) break; // no hay mas objetos

		// tomo lo que pueda del objeto seleccionado
		if (items[idx_mejor].peso <= capacidad) {
			valor_total += items[idx_mejor].valor;
			capacidad -= items[idx_mejor].peso;
			items[idx_mejor].tomado = true;
		} else {
			double fraccion = (double)capacidad / (double)items[idx_mejor].peso;
			valor_total += items[idx_mejor].valor * fraccion;
			capacidad = 0;
			items[idx_mejor].tomado = true; // considerarlo tomado parcialmente
		}
	}
	return valor_total;
}

int main() {
	// Conjunto de prueba: 30 items
vector<int> valor = {10,20,30,25,40,35,45,50,8,38,60,5,70,65,18,22,14,55,33
                    ,48,27,19,44,58,61,72,13,17,29,36};
vector<int> peso  = {3,4,6,5,8,7,9,11,2,10,12,1,14,13,4,6,3,9,5,10,7,4,8,11,
                    12,15,2,3,6,7};
	int W = 150;

	cout << "Programa 4: Mochila fraccional - Sin optimizacion" << endl;

	auto inicio = high_resolution_clock::now();
	double resultado = mochila_fraccional_sin_opt(W, peso, valor);
	auto fin = high_resolution_clock::now();

	cout << "Valor maximo (fraccional sin optimizacion): " << resultado << endl;
	auto duracion = duration_cast<microseconds>(fin - inicio).count();
	cout << "Duracion: " << duracion << " microsegundos" << endl;

	return 0;
}
