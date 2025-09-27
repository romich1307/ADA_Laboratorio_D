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

// Función para el algoritmo de Selection Sort
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

// Función para el algoritmo de Insertion Sort
void insertionSort(vector<float>& lista, int& comparaciones, int& intercambios) {
    int n = lista.size();
    comparaciones = 0;
    intercambios = 0;

    for (int i = 1; i < n; i++) {
        float clave = lista[i];
        int j = i - 1;

        // Mover los elementos que son mayores que clave a una posición adelante
        while (j >= 0 && lista[j] > clave) {
            lista[j + 1] = lista[j];
            j--;
            comparaciones++;
            intercambios++;
        }
        lista[j + 1] = clave; // Insertar el elemento en su lugar adecuado
        comparaciones++; // Se hace una comparación al final
    }
}

// Se generará un vector de acuerdo al tamaño de datos
vector<float> generarVectorAleatorio(int tamaño) {
    vector<float> vec(tamaño);
    random_device rd;
    mt19937 generador(rd());
    uniform_real_distribution<float> rango(1.0f, 1000.0f);
    for (int i = 0; i < tamaño; i++) 
    { vec[i] = rango(generador); }
    return vec;
}

// Función que recibe arreglo y tipo de ordenamiento
void eleccionYOrdenamiento(vector<float>& lista, const string& tipoOrdenamiento, int& comparaciones, int& intercambios) {
    if (tipoOrdenamiento == "selection") {
        selectionSort(lista, comparaciones, intercambios);
    } else if (tipoOrdenamiento == "insertion") {
        insertionSort(lista, comparaciones, intercambios);
    } else {
        throw runtime_error("Tipo de ordenamiento no soportado. Usa 'selection' o 'insertion'.");
    }
}

// Evaluar rendimiento
void evaluarRendimiento(int tamaño, const string& tipoOrdenamiento) {
    cout << "\nCon " << tamaño << " elementos (" << tipoOrdenamiento << ")\n";
    vector<float> lista = generarVectorAleatorio(tamaño);
    int comparaciones = 0, intercambios = 0;

    auto inicio = steady_clock::now();
    eleccionYOrdenamiento(lista, tipoOrdenamiento, comparaciones, intercambios);
    auto fin = steady_clock::now();

    auto tiempo = duration_cast<microseconds>(fin - inicio).count();
    cout << "Tiempo: " << tiempo << " microsegundos ("<< fixed << setprecision(2) << tiempo / 1000.0 << " ms)\n";
    cout << "Comparaciones: " << comparaciones << "\n";
    cout << "Intercambios: " << intercambios << "\n";
}

int main() {
    // Evaluación de rendimiento para ambos tipos de ordenamiento
    cout << "EVALUACION DE RENDIMIENTO - SELECTION Y INSERTION SORT\n";
    evaluarRendimiento(1000, "selection");
    evaluarRendimiento(5000, "selection");
    evaluarRendimiento(10000, "selection");

    evaluarRendimiento(1000, "insertion");
    evaluarRendimiento(5000, "insertion");
    evaluarRendimiento(10000, "insertion");

    cout << "\nPRUEBA \n";
    cout << "Ingrese el tamaño del arreglo a ordenar: ";
    int n;  
    cin >> n;

    vector<float> lista(n);
    cout << "Ingrese los elementos del arreglo:\n";
    for (int i = 0; i < n; i++) cin >> lista[i];

    string tipo = "insertion";  // Puedes cambiar a "selection" si lo prefieres
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