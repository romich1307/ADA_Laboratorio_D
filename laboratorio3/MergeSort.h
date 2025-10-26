#ifndef MERGESORT_H
#define MERGESORT_H

#include "Estudiante.h"
#include <vector>

class MergeSort {
private:
    // Función auxiliar para dividir y conquistar
    static void mergeSort(std::vector<Estudiante>& arr, int inicio, int fin);
    
    // Función para combinar dos subarreglos ordenados
    static void merge(std::vector<Estudiante>& arr, int inicio, int medio, int fin);
    
public:
    // Función pública para ordenar el vector de estudiantes
    static void ordenar(std::vector<Estudiante>& estudiantes);
    
    // Función para ordenar en orden descendente
    static void ordenarDescendente(std::vector<Estudiante>& estudiantes);
    
    // Función auxiliar para orden descendente
    static void mergeSortDescendente(std::vector<Estudiante>& arr, int inicio, int fin);
    static void mergeDescendente(std::vector<Estudiante>& arr, int inicio, int medio, int fin);
};

#endif // MERGESORT_H
