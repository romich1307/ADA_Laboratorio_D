//Crea una función que reciba un arreglo y un tipo de ordenamiento ("insertion" o "selection") 
// y aplique el algoritmo correspondiente. 
// Evalúa el rendimiento con 1000, 5000 y 10000 elementos aleatorios. 


#include <iostream>
#include <vector>
#include <string>
#include <chrono>  // Para medir tiempo de ejecución
#include <random>  // Para generar números aleatorios
#include <iomanip> // Para formatear la salida
using namespace std;
using namespace std::chrono;

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

// Se generara un vector de acuerdo al tamano de datos
vector<float> generarVectorAleatorio(int tamaño) {
    vector<float> vec(tamaño);
    random_device rd;
    mt19937 generador(rd());
    uniform_real_distribution<float> rango(1.0f, 1000.0f);
    for (int i = 0; i < tamaño; i++) 
    {vec[i] = rango(generador);}
    return vec;
}

// Se evalua rendimiento
void evaluarRendimiento(int tamaño) {
    cout << "\n Con " << tamaño << " elementos \n";
    vector<float> lista = generarVectorAleatorio(tamaño);
    int comparaciones = 0, intercambios = 0;

    auto inicio = steady_clock::now();
    selectionSort(lista, comparaciones, intercambios);
    auto fin = steady_clock::now();

    auto tiempo = duration_cast<microseconds>(fin - inicio).count();
    cout << "Tiempo: " << tiempo << " microsegundos ("<< fixed << setprecision(2) << tiempo / 1000.0 << " ms)\n";
    cout << "Comparaciones: " << comparaciones << "\n";
    cout << "Intercambios: " << intercambios << "\n";
}

// Recibe arreglo y tipo de ordenamiento
void eleccionYOrdenamiento(vector<float>& lista,const string& tipoOrdenamiento,int& comparaciones,int& intercambios) {
    if (tipoOrdenamiento == "selection") {
        selectionSort(lista, comparaciones, intercambios);
    } else {
        throw runtime_error("Tipo de ordenamiento no soportado. Usa 'selection'.");
    }
}

int main() {
    cout << "EVALUACION DE RENDIMIENTO - SELECTION SORT \n";
    evaluarRendimiento(1000);
    evaluarRendimiento(5000);
    evaluarRendimiento(10000);

    cout << "\nPRUEBA \n";
    cout << "Ingrese el tamaño del arreglo a ordenar: ";
    int n;  cin >> n;

    vector<float> lista(n);
    cout << "Ingrese los elementos del arreglo:\n";
    for (int i = 0; i < n; i++) cin >> lista[i];

    string tipo = "selection"; 
    int comparaciones = 0, intercambios = 0;

    cout << "\nLista original: [ ";
    for (float x : lista) cout << x << " ";
    cout << "]\n";

    auto inicio = steady_clock::now();
    eleccionYOrdenamiento(lista, tipo, comparaciones, intercambios);
    auto fin = steady_clock::now();
    auto us = duration_cast<microseconds>(fin - inicio).count();

    cout << "Lista ordenada: [ ";
    for (float x : lista) cout << x << " ";
    cout << "]\n";

    cout << "Tiempo: " << us << " microsegundos ("<< fixed << setprecision(2) << us / 1000.0 << " ms)\n";
    cout << "Comparaciones: " << comparaciones << "\n";
    cout << "Intercambios: " << intercambios << "\n";

    return 0;
}