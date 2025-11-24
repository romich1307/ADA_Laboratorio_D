// Programa 5: Subir escalones (1,2,3)
// Version bruta (sin memo)
// Descripcion en primera persona:
// - Yo calculo el numero de formas de subir n escalones
//   permitiendo que cada paso sea de 1, 2 o 3 escalones.
// - Yo implemento la funcion 'formas' como una version recursiva
//   que explora todas las combinaciones posibles. Incluyo esta
//   version con fines didacticos para comparar tiempos con
//   las versiones con memoizacion y tabulacion.
// - Nota: yo entiendo que la complejidad es exponencial O(3^n)
//   en el peor caso, por eso esta version es lenta para n grandes.

#include <iostream>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

// Version bruta: f(n) = f(n-1) + f(n-2) + f(n-3)
// Mi descripcion de la funcion:
// - Si n < 0: yo devuelvo 0 porque no hay forma valida
// - Si n == 0: yo devuelvo 1 porque la combinacion vacia es una forma valida
// - En otro caso: yo sumo las formas que terminan con un paso de 1, 2 o 3
int formas(int n) {
	if (n < 0) return 0;    // devuelvo 0 para casos invalidos
	if (n == 0) return 1;   // devuelvo 1 para la combinacion vacia
	// llamo recursivamente para sumar las formas con pasos 1,2 y 3
	return formas(n-1) + formas(n-2) + formas(n-3);
}

int main() {
	// Entrada: yo leo n desde la entrada estandar
	int n;
	cout << "Ingrese n: ";
	cin >> n;

	// Medir tiempo de ejecucion: inicio
	auto inicio = high_resolution_clock::now();
	// Calculo principal: yo llamo a la funcion recursiva
	int resultado = formas(n);
	// Medir tiempo de ejecucion: fin
	auto fin = high_resolution_clock::now();

	// Salida: yo muestro el titulo, el resultado y la duracion en microsegundos
	cout << "Programa de formas de subir escalones - sin optimizacion" << endl;
	cout << "Formas (bruto): " << resultado << endl;
	auto duracion = duration_cast<microseconds>(fin - inicio).count();
	cout << "La duracion del algoritmo bruto es: " << duracion << " microsegundos" << endl;

	return 0;
}
