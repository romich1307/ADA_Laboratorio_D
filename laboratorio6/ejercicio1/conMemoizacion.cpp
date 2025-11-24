#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std::chrono;
using namespace std;

// Funcion para calcular el n-ésimo número 
// de Fibonacci utilizando memorización
int nthFibonacciUtil(int n, vector<int>& memo) {
    // Caso base: si n es 0 o 1, retornar n
    if (n <= 1) {
        return n;
    }

    // Verificar si el resultado ya está en la tabla de memo
    if (memo[n] != -1) {
        return memo[n];
    }

    // Caso recursivo: calcular el número de Fibonacci
    // y almacenarlo en la tabla de en la tabla de memoización
    memo[n] = nthFibonacciUtil(n - 1, memo) + nthFibonacciUtil(n - 2, memo);

    return memo[n];
}

// Función principal para calcular el n-ésimo número de Fibonacci
int nthFibonacci(int n) {

    // Crear una tabla de memoización e inicializar con -1
    vector<int> memo(n + 1, -1);

    // Llamar a la función de utilidad
    return nthFibonacciUtil(n, memo);
}

int main() {
    int n;
    cout << "Ingrese el valor de n: ";
    cin >> n;
    auto inicio = high_resolution_clock::now();
    int result = nthFibonacci(n);
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<microseconds>(fin - inicio).count();
    cout << "El resultado del Fibonacci para " << n << " es: " << result << endl;
    cout << "La duracion del fibonacci con memorizacion es: " << duracion << " microsegundos" << endl;
    return 0;
}