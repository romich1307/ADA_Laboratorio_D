// Programa 5: Subir escalones (1,2,3) - Tabulacion
// Yo calculo el numero de formas de llegar al escalon n usando una tabla
// bottom-up. Esta version es iterativa y evita recursion.

#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

// formas_tabulacion: devuelve f(n) con f(0)=1, f(1)=1, f(2)=2
int formas_tabulacion(int n) {
    if (n < 0) return 0;
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;
    vector<int> arreglo(n + 1, 0);
    arreglo[0] = 1;
    arreglo[1] = 1;
    arreglo[2] = 2;
    for (int i = 3; i <= n; ++i) {
        arreglo[i] = arreglo[i-1] + arreglo[i-2] + arreglo[i-3];
    }
    return arreglo[n];
}

int main() {
    int n;
    cout << "Ingrese n: ";
    cin >> n;

    auto inicio_tiempo = high_resolution_clock::now();
    int resultado = formas_tabulacion(n);
    auto fin_tiempo = high_resolution_clock::now();

    cout << "Programa de formas de subir escalones - Tabulacion" << endl;
    cout << "Formas: " << resultado << endl;
    auto duracion_us = duration_cast<microseconds>(fin_tiempo - inicio_tiempo).count();
    cout << "Duracion: " << duracion_us << " microsegundos" << endl;
    return 0;
}