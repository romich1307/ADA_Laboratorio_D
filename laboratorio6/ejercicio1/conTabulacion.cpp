//Implementa un algoritmo para calcular el n-ésimo número de la serie de
//  Fibonacci utilizando programación dinámica con tabulacion.
#include <iostream>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

// Creamos una funcion de fibonacchi con programacion dinamica
// usando la tabulacion

int tabulacionFibonacci(int n){
    // Caso base de fibonacci
    if (n <= 1)
        return n;

    // Creamos un vector para almacenar los numeros de Fibonacci
    vector<int> arreglo(n + 1);

    // Inicializamos los dos primeros numeros de Fibonacci
    arreglo[0] = 0;
    arreglo[1] = 1;

    // Llenamos el vector de manera iterativa
    for (int i = 2; i <= n; ++i){

        // Calculamos el siguiente numero de Fibonacci
        arreglo[i] = arreglo[i - 1] + arreglo[i - 2];
    }

    // Retornamos el n-ésimo numero de Fibonacci
    return arreglo[n];
}

int main(){
    int n;
    cout << "Ingrese el valor de n: ";
    cin >> n;
    // Medimos el tiempo de ejecucion
    auto inicio = high_resolution_clock::now();
    int resultado = tabulacionFibonacci(n);
    auto fin = high_resolution_clock::now();
    cout << resultado << endl;
    auto duracion = duration_cast<microseconds>(fin - inicio).count();
    cout << "La duracion del algoritmo con tabulacion es: " << duracion << " microsegundos" << endl;

    return 0;
}