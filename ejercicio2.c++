//Implementa el algoritmo Selection Sort sobre un arreglo de números flotantes.
//Muestra el número de comparaciones e intercambios realizado

#include <iostream>
#include <vector>
using namespace std;

void selectionSort(vector<int>& lista) {
    int n = lista.size();

    for (int i = 0; i < n - 1; i++) {
        int indiceMinimo = i;
        for (int j = i + 1; j < n; j++) {
            if (lista[j] < lista[indiceMinimo]) {
                indiceMinimo = j;
            }
        }
        // Intercambiamos
        int temporal = lista[i];
        lista[i] = lista[indiceMinimo];
        lista[indiceMinimo] = temporal;
    }
}

int main() {
    vector<int> lista = {5, 4, 4, 3, 2};

    cout << "El elemento minimo de la lista es: " << endl;

    selectionSort(lista);

    cout << "Lista ordenada: [ ";
    for (int i = 0; i < lista.size(); i++) {
        cout << lista[i] << " ";
    }
    cout << "]" << endl;

    return 0;
}
