#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>    // Para medir el tiempo de ejecución con precisión

using namespace std;
using namespace chrono;

// Función que implementa el algoritmo de Insertion Sort
void ordenamientoPorInsercion(vector<string>& lista) {
    int tamaño = lista.size();
    for (int i = 1; i < tamaño; i++) {
        string clave = lista[i]; // Elemento actual a insertar
        int j = i - 1;
        
        // Mover los elementos que son mayores que clave a una posición adelante
        while (j >= 0 && lista[j] > clave) {
            lista[j + 1] = lista[j];
            j--;
        }
        lista[j + 1] = clave; // Insertar el elemento en su lugar adecuado
    }
}

// Función para imprimir la lista de nombres
void imprimirLista(const vector<string>& lista) {
    for (const auto& nombre : lista) {
        cout << nombre << " ";
    }
    cout << endl;
}

int main() {
    // Caso 1: Lista ya ordenada
    vector<string> listaOrdenada = {"Ana", "Carlos", "David", "Eva", "Laura"};
    cout << "Lista ya ordenada:" << endl;
    imprimirLista(listaOrdenada);

    // Medir el tiempo de ejecución para lista ordenada
    auto inicio = high_resolution_clock::now();
    ordenamientoPorInsercion(listaOrdenada);
    auto fin = high_resolution_clock::now();
    cout << "Lista ordenada usando Insertion Sort:" << endl;
    imprimirLista(listaOrdenada);
    auto duracion = duration_cast<microseconds>(fin - inicio); // Convertir a microsegundos
    cout << "Tiempo de ejecución para lista ordenada: " << duracion.count() << " microsegundos" << endl;

    // Caso 2: Lista en orden inverso
    vector<string> listaInvertida = {"Laura", "Eva", "David", "Carlos", "Ana"};
    cout << "\nLista en orden inverso:" << endl;
    imprimirLista(listaInvertida);

    // Medir el tiempo de ejecución para lista inversa
    inicio = high_resolution_clock::now();
    ordenamientoPorInsercion(listaInvertida);
    fin = high_resolution_clock::now();
    cout << "Lista ordenada usando Insertion Sort:" << endl;
    imprimirLista(listaInvertida);
    duracion = duration_cast<microseconds>(fin - inicio);
    cout << "Tiempo de ejecución para lista en orden inverso: " << duracion.count() << " microsegundos" << endl;

    // Caso 3: Lista aleatoria
    vector<string> listaAleatoria = {"Eva", "Carlos", "Ana", "Laura", "David"};
    cout << "\nLista aleatoria:" << endl;
    imprimirLista(listaAleatoria);

    // Medir el tiempo de ejecución para lista aleatoria
    inicio = high_resolution_clock::now();
    ordenamientoPorInsercion(listaAleatoria);
    fin = high_resolution_clock::now();
    cout << "Lista ordenada usando Insertion Sort:" << endl;
    imprimirLista(listaAleatoria);
    duracion = duration_cast<microseconds>(fin - inicio);
    cout << "Tiempo de ejecución para lista aleatoria: " << duracion.count() << " microsegundos" << endl;

    return 0;
}
