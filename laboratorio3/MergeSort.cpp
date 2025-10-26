#include "MergeSort.h"
#include <vector>

// Función pública para ordenar (ascendente por promedio)
void MergeSort::ordenar(std::vector<Estudiante>& estudiantes) {
    if (estudiantes.size() <= 1) return;
    mergeSort(estudiantes, 0, estudiantes.size() - 1);
}

// Función recursiva de merge sort (ascendente)
void MergeSort::mergeSort(std::vector<Estudiante>& arr, int inicio, int fin) {
    if (inicio < fin) {
        int medio = inicio + (fin - inicio) / 2;
        
        // Dividir recursivamente
        mergeSort(arr, inicio, medio);
        mergeSort(arr, medio + 1, fin);
        
        // Combinar los subarreglos ordenados
        merge(arr, inicio, medio, fin);
    }
}

// Función para combinar dos subarreglos ordenados (ascendente)
void MergeSort::merge(std::vector<Estudiante>& arr, int inicio, int medio, int fin) {
    int n1 = medio - inicio + 1;
    int n2 = fin - medio;
    
    // Crear vectores temporales
    std::vector<Estudiante> izquierda(n1);
    std::vector<Estudiante> derecha(n2);
    
    // Copiar datos a los vectores temporales
    for (int i = 0; i < n1; i++) {
        izquierda[i] = arr[inicio + i];
    }
    for (int j = 0; j < n2; j++) {
        derecha[j] = arr[medio + 1 + j];
    }
    
    // Combinar los vectores temporales de vuelta en arr[inicio..fin]
    int i = 0, j = 0, k = inicio;
    
    while (i < n1 && j < n2) {
        if (izquierda[i] <= derecha[j]) {
            arr[k] = izquierda[i];
            i++;
        } else {
            arr[k] = derecha[j];
            j++;
        }
        k++;
    }
    
    // Copiar los elementos restantes de izquierda[], si los hay
    while (i < n1) {
        arr[k] = izquierda[i];
        i++;
        k++;
    }
    
    // Copiar los elementos restantes de derecha[], si los hay
    while (j < n2) {
        arr[k] = derecha[j];
        j++;
        k++;
    }
}

// Función pública para ordenar descendente
void MergeSort::ordenarDescendente(std::vector<Estudiante>& estudiantes) {
    if (estudiantes.size() <= 1) return;
    mergeSortDescendente(estudiantes, 0, estudiantes.size() - 1);
}

// Función recursiva de merge sort (descendente)
void MergeSort::mergeSortDescendente(std::vector<Estudiante>& arr, int inicio, int fin) {
    if (inicio < fin) {
        int medio = inicio + (fin - inicio) / 2;
        
        // Dividir recursivamente
        mergeSortDescendente(arr, inicio, medio);
        mergeSortDescendente(arr, medio + 1, fin);
        
        // Combinar los subarreglos ordenados
        mergeDescendente(arr, inicio, medio, fin);
    }
}

// Función para combinar dos subarreglos ordenados (descendente)
void MergeSort::mergeDescendente(std::vector<Estudiante>& arr, int inicio, int medio, int fin) {
    int n1 = medio - inicio + 1;
    int n2 = fin - medio;
    
    // Crear vectores temporales
    std::vector<Estudiante> izquierda(n1);
    std::vector<Estudiante> derecha(n2);
    
    // Copiar datos a los vectores temporales
    for (int i = 0; i < n1; i++) {
        izquierda[i] = arr[inicio + i];
    }
    for (int j = 0; j < n2; j++) {
        derecha[j] = arr[medio + 1 + j];
    }
    
    // Combinar los vectores temporales de vuelta en arr[inicio..fin]
    int i = 0, j = 0, k = inicio;
    
    while (i < n1 && j < n2) {
        if (izquierda[i] >= derecha[j]) {  // Cambio aquí para orden descendente
            arr[k] = izquierda[i];
            i++;
        } else {
            arr[k] = derecha[j];
            j++;
        }
        k++;
    }
    
    // Copiar los elementos restantes de izquierda[], si los hay
    while (i < n1) {
        arr[k] = izquierda[i];
        i++;
        k++;
    }
    
    // Copiar los elementos restantes de derecha[], si los hay
    while (j < n2) {
        arr[k] = derecha[j];
        j++;
        k++;
    }
}
