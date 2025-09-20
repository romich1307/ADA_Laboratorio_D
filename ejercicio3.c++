//Crea una función que reciba un arreglo y un tipo de ordenamiento ("insertion" o "selection") 
// y aplique el algoritmo correspondiente. 
// Evalúa el rendimiento con 1000, 5000 y 10000 elementos aleatorios. 


#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

void selectionSort(vector<float>& lista, int& comparaciones, int& intercambios) {
    int n = lista.size();
    comparaciones = 0;
    intercambios = 0;

    for (int i = 0; i < n - 1; i++) {
        int indiceMinimo = i;
        for (int j = i + 1; j < n; j++) {
            comparaciones++; // Contamos cada comparación
            if (lista[j] < lista[indiceMinimo]) {
                indiceMinimo = j;
            }
        }
        
        if (indiceMinimo != i) {
            intercambios++; // Contamos cada intercambio
            float temporal = lista[i];
            lista[i] = lista[indiceMinimo];
            lista[indiceMinimo] = temporal;
        }
    }
}

void eleccionYOrdenamiento(vector<float>& lista, string tipoOrdenamiento) {
    if (tipoOrdenamiento == "selectionSort") {
        selectionSort(lista, comparaciones, intercambios);
    } else {
        cout << "Tipo de ordenamiento no soportado." << endl;
    }
}
