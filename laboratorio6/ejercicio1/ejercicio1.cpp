//Implementamos un algoritmo para calcular el n-ésimo número de la serie de 
// Fibonacci utilizando recursión simple.
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Creamos una funcion de fibonacchi sin la necesidad
// de usar programacion dinamica
int fibonacchi(int n){

    // Caso base de fiboncci
    if (n ==0 || n ==1){
        return n;
    }
    //Llamos por recursion a la funcion fibonacchi
    return fibonacchi(n - 1) + fibonacchi(n - 2);
}

int main(){
    int n ;
    cout << "Ingrese el valor de n: ";
    cin>>n;
    //Calculamos el tiempo de ejecucion
    auto inicio = high_resolution_clock::now();
    int resultado = fibonacchi (n);
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<microseconds>(fin - inicio).count();
    cout <<"El resultado del fibonnaci para "<< n << " es: "<< resultado << endl;
    cout << "Duracion: " << duracion << " microsegundos" << endl;
    return 0;
}