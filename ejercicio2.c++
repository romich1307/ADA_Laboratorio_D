//Implementa el algoritmo Selection Sort sobre un arreglo de números flotantes.
//Muestra el número de comparaciones e intercambios realizado

#include <iostream>
#include <vector>
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

int main() {
    vector<float> lista = {5.2, 4.1, 4.8, 3.3, 2.7};
    int comparaciones, intercambios;

    cout << "Lista original: [ ";
    for (int i = 0; i < lista.size(); i++) {
        cout << lista[i] << " ";
    }
    cout << "]" << endl;

    selectionSort(lista, comparaciones, intercambios);

    cout << "Lista ordenada: [ ";
    for (int i = 0; i < lista.size(); i++) {
        cout << lista[i] << " ";
    }
    cout << "]" << endl;

    cout << "Numero de comparaciones: " << comparaciones << endl;
    cout << "Numero de intercambios: " << intercambios << endl;

    return 0;
}
