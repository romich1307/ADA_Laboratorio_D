// Programa 5: Subir escalones (1,2,3) - Memoizacion
// Yo implemento la version recursiva con memoizacion para evitar recomputos.

#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

// funcion recursiva con memo (tabla) que devuelve f(n)
int formas_memo_rec(int n, vector<int> &arreglo_memo) {
    if (n < 0) return 0;
    if (n == 0) return 1;
    if (arreglo_memo[n] != -1) return arreglo_memo[n];
    arreglo_memo[n] = formas_memo_rec(n-1, arreglo_memo) + formas_memo_rec(n-2, arreglo_memo) + formas_memo_rec(n-3, arreglo_memo);
    return arreglo_memo[n];
}

int formas_memoizacion(int n) {
    if (n < 0) return 0;
    vector<int> arreglo_memo(n + 1, -1);
    return formas_memo_rec(n, arreglo_memo);
}

int main() {
    int n;
    cout << "Ingrese n: ";
    cin >> n;

    auto inicio_tiempo = high_resolution_clock::now();
    int resultado = formas_memoizacion(n);
    auto fin_tiempo = high_resolution_clock::now();

    cout << "Programa de formas de subir escalones - Memoizacion" << endl;
    cout << "Formas: " << resultado << endl;
    auto duracion_us = duration_cast<microseconds>(fin_tiempo - inicio_tiempo).count();
    cout << "Duracion: " << duracion_us << " microsegundos" << endl;
    return 0;
}