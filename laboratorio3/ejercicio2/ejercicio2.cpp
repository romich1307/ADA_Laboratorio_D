#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int comparaciones = 0;

// Función para intercambiar dos elementos
void intercambiar(float &a, float &b) {
    float temp = a;
    a = b;
    b = temp;
}

// Partición con pivote fijo (primer elemento)
int particionPivoteFijo(vector<float> &arr, int bajo, int alto) {
    float pivote = arr[bajo];
    int i = bajo + 1;
    
    for (int j = bajo + 1; j <= alto; j++) {
        comparaciones++;
        if (arr[j] <= pivote) {
            intercambiar(arr[i], arr[j]);
            i++;
        }
    }
    intercambiar(arr[bajo], arr[i - 1]);
    return i - 1;
}

// QuickSort con pivote fijo
void quickSortPivoteFijo(vector<float> &arr, int bajo, int alto) {
    if (bajo < alto) {
        int pi = particionPivoteFijo(arr, bajo, alto);
        quickSortPivoteFijo(arr, bajo, pi - 1);
        quickSortPivoteFijo(arr, pi + 1, alto);
    }
}

// Partición con pivote aleatorio
int particionPivoteAleatorio(vector<float> &arr, int bajo, int alto) {
    int indiceAleatorio = bajo + rand() % (alto - bajo + 1);
    intercambiar(arr[bajo], arr[indiceAleatorio]);
    
    float pivote = arr[bajo];
    int i = bajo + 1;
    
    for (int j = bajo + 1; j <= alto; j++) {
        comparaciones++;
        if (arr[j] <= pivote) {
            intercambiar(arr[i], arr[j]);
            i++;
        }
    }
    intercambiar(arr[bajo], arr[i - 1]);
    return i - 1;
}

// QuickSort con pivote aleatorio
void quickSortPivoteAleatorio(vector<float> &arr, int bajo, int alto) {
    if (bajo < alto) {
        int pi = particionPivoteAleatorio(arr, bajo, alto);
        quickSortPivoteAleatorio(arr, bajo, pi - 1);
        quickSortPivoteAleatorio(arr, pi + 1, alto);
    }
}

// Función para mostrar el arreglo
void mostrarArreglo(vector<float> &arr) {
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Función para generar arreglo aleatorio
vector<float> generarArregloAleatorio(int n) {
    vector<float> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = (float)(rand() % 100) + (float)(rand() % 100) / 100.0f;
    }
    return arr;
}

// Función para generar arreglo ordenado
vector<float> generarArregloOrdenado(int n) {
    vector<float> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = (float)(i + 1);
    }
    return arr;
}

// Función para generar arreglo en orden reverso
vector<float> generarArregloReverso(int n) {
    vector<float> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = (float)(n - i);
    }
    return arr;
}

int main() {
    srand(time(0));
    
    cout << "=== ANÁLISIS DE QUICKSORT ===" << endl;
    cout << "Pivote Fijo - Pivote Aleatorio" << endl << endl;
    
    int n;
    cout << "Ingrese el tamaño del arreglo: ";
    cin >> n;
    
    // Prueba con datos aleatorios
    cout << "\n--- DATOS ALEATORIOS ---" << endl;
    vector<float> arr1 = generarArregloAleatorio(n);
    vector<float> arr2 = arr1; 
    
    cout << "Arreglo original (primeros 10 elementos): ";
    for (int i = 0; i < min(10, n); i++) {
        cout << arr1[i] << " ";
    }
    cout << endl;
    
    // Pivote fijo
    comparaciones = 0;
    quickSortPivoteFijo(arr1, 0, n - 1);
    int comp_fijo_aleatorio = comparaciones;
    cout << "Pivote fijo - Comparaciones: " << comp_fijo_aleatorio << endl;
    
    // Pivote aleatorio
    comparaciones = 0;
    quickSortPivoteAleatorio(arr2, 0, n - 1);
    int comp_aleatorio_aleatorio = comparaciones;
    cout << "Pivote aleatorio - Comparaciones: " << comp_aleatorio_aleatorio << endl;
    
    // Prueba con datos ordenados
    cout << "\n--- DATOS ORDENADOS ---" << endl;
    vector<float> arr3 = generarArregloOrdenado(n);
    vector<float> arr4 = arr3;
    
    // Pivote fijo
    comparaciones = 0;
    quickSortPivoteFijo(arr3, 0, n - 1);
    int comp_fijo_ordenado = comparaciones;
    cout << "Pivote fijo - Comparaciones: " << comp_fijo_ordenado << endl;
    
    // Pivote aleatorio
    comparaciones = 0;
    quickSortPivoteAleatorio(arr4, 0, n - 1);
    int comp_aleatorio_ordenado = comparaciones;
    cout << "Pivote aleatorio - Comparaciones: " << comp_aleatorio_ordenado << endl;
    
    // Prueba con datos en orden reverso
    cout << "\n--- DATOS EN ORDEN REVERSO ---" << endl;
    vector<float> arr5 = generarArregloReverso(n);
    vector<float> arr6 = arr5;
    
    // Pivote fijo
    comparaciones = 0;
    quickSortPivoteFijo(arr5, 0, n - 1);
    int comp_fijo_reverso = comparaciones;
    cout << "Pivote fijo - Comparaciones: " << comp_fijo_reverso << endl;
    
    // Pivote aleatorio
    comparaciones = 0;
    quickSortPivoteAleatorio(arr6, 0, n - 1);
    int comp_aleatorio_reverso = comparaciones;
    cout << "Pivote aleatorio - Comparaciones: " << comp_aleatorio_reverso << endl;
    
    // Mostrar resultado final
    cout << "\nArreglo ordenado (primeros 10 elementos): ";
    for (int i = 0; i < min(10, n); i++) {
        cout << arr1[i] << " ";
    }
    cout << endl;
    
    return 0;
}
