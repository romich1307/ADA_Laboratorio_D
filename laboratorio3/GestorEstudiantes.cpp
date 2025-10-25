#include "GestorEstudiantes.h"
#include "MergeSort.h"
#include <iostream>
#include <string>
#include <limits>

// Constructor
GestorEstudiantes::GestorEstudiantes() {}

// Agregar un estudiante a la lista
void GestorEstudiantes::agregarEstudiante(const Estudiante& estudiante) {
    estudiantes.push_back(estudiante);
}

// Ingresar estudiantes desde la consola
void GestorEstudiantes::ingresarEstudiantesDesdeConsola() {
    int cantidad;
    std::cout << "¿Cuántos estudiantes desea ingresar? ";
    
    while (!(std::cin >> cantidad) || cantidad <= 0) {
        std::cout << "Por favor, ingrese un número válido mayor que 0: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    std::cin.ignore(); // Limpiar el buffer
    
    for (int i = 0; i < cantidad; i++) {
        std::string codigo, nombre;
        float promedio;
        
        std::cout << "\n--- Estudiante " << (i + 1) << " ---" << std::endl;
        
        std::cout << "Código: ";
        std::getline(std::cin, codigo);
        
        std::cout << "Nombre: ";
        std::getline(std::cin, nombre);
        
        std::cout << "Promedio ponderado: ";
        while (!(std::cin >> promedio) || promedio < 0.0 || promedio > 20.0) {
            std::cout << "Por favor, ingrese un promedio válido (0.0 - 20.0): ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(); // Limpiar el buffer
        
        agregarEstudiante(Estudiante(codigo, nombre, promedio));
    }
    
    std::cout << "\n¡Estudiantes ingresados correctamente!" << std::endl;
}

// Mostrar todos los estudiantes
void GestorEstudiantes::mostrarEstudiantes() const {
    if (estudiantes.empty()) {
        std::cout << "No hay estudiantes registrados." << std::endl;
        return;
    }
    
    std::cout << "\n=== LISTA DE ESTUDIANTES ===" << std::endl;
    for (const auto& estudiante : estudiantes) {
        estudiante.mostrar();
    }
    std::cout << "=============================" << std::endl;
}

// Mostrar estudiantes con numeración
void GestorEstudiantes::mostrarEstudiantesNumerados() const {
    if (estudiantes.empty()) {
        std::cout << "No hay estudiantes registrados." << std::endl;
        return;
    }
    
    std::cout << "\n=== LISTA DE ESTUDIANTES ===" << std::endl;
    for (size_t i = 0; i < estudiantes.size(); i++) {
        std::cout << (i + 1) << ". ";
        estudiantes[i].mostrar();
    }
    std::cout << "=============================" << std::endl;
}

// Ordenar por promedio (ascendente) usando MergeSort
void GestorEstudiantes::ordenarPorPromedio() {
    if (estudiantes.empty()) {
        std::cout << "No hay estudiantes para ordenar." << std::endl;
        return;
    }
    
    std::cout << "Ordenando estudiantes por promedio ponderado (ascendente)..." << std::endl;
    MergeSort::ordenar(estudiantes);
    std::cout << "¡Ordenamiento completado!" << std::endl;
}

// Ordenar por promedio (descendente) usando MergeSort
void GestorEstudiantes::ordenarPorPromedioDescendente() {
    if (estudiantes.empty()) {
        std::cout << "No hay estudiantes para ordenar." << std::endl;
        return;
    }
    
    std::cout << "Ordenando estudiantes por promedio ponderado (descendente)..." << std::endl;
    MergeSort::ordenarDescendente(estudiantes);
    std::cout << "¡Ordenamiento completado!" << std::endl;
}

// Getters
std::vector<Estudiante>& GestorEstudiantes::getEstudiantes() {
    return estudiantes;
}

const std::vector<Estudiante>& GestorEstudiantes::getEstudiantes() const {
    return estudiantes;
}

int GestorEstudiantes::getCantidadEstudiantes() const {
    return estudiantes.size();
}

// Limpiar la lista de estudiantes
void GestorEstudiantes::limpiarLista() {
    estudiantes.clear();
    std::cout << "Lista de estudiantes limpiada." << std::endl;
}
