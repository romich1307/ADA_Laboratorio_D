#include "GestorEstudiantes.h"
#include "Estudiante.h"
#include "MergeSort.h"
#include <iostream>
#include <limits>

void mostrarMenu() {
    std::cout << "\n========== SISTEMA DE GESTIÓN DE ESTUDIANTES ==========" << std::endl;
    std::cout << "1. Ingresar estudiantes" << std::endl;
    std::cout << "2. Mostrar estudiantes" << std::endl;
    std::cout << "3. Ordenar por promedio (ascendente)" << std::endl;
    std::cout << "4. Ordenar por promedio (descendente)" << std::endl;
    std::cout << "5. Agregar un estudiante individual" << std::endl;
    std::cout << "6. Limpiar lista de estudiantes" << std::endl;
    std::cout << "7. Salir" << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "Seleccione una opción: ";
}

void agregarEstudianteIndividual(GestorEstudiantes& gestor) {
    std::string codigo, nombre;
    float promedio;
    
    std::cout << "\n--- Agregar Estudiante Individual ---" << std::endl;
    
    std::cin.ignore(); // Limpiar buffer si es necesario
    
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
    
    gestor.agregarEstudiante(Estudiante(codigo, nombre, promedio));
    std::cout << "¡Estudiante agregado correctamente!" << std::endl;
}

void mostrarEstadisticas(const GestorEstudiantes& gestor) {
    if (gestor.getCantidadEstudiantes() == 0) {
        std::cout << "No hay estudiantes registrados." << std::endl;
        return;
    }
    
    const auto& estudiantes = gestor.getEstudiantes();
    float suma = 0.0f;
    float maximo = estudiantes[0].getPromedioPonderado();
    float minimo = estudiantes[0].getPromedioPonderado();
    
    for (const auto& est : estudiantes) {
        float promedio = est.getPromedioPonderado();
        suma += promedio;
        if (promedio > maximo) maximo = promedio;
        if (promedio < minimo) minimo = promedio;
    }
    
    float promedioCurso = suma / estudiantes.size();
    
    std::cout << "\n=== ESTADÍSTICAS ===" << std::endl;
    std::cout << "Total de estudiantes: " << estudiantes.size() << std::endl;
    std::cout << "Promedio del curso: " << std::fixed << std::setprecision(2) << promedioCurso << std::endl;
    std::cout << "Promedio más alto: " << std::fixed << std::setprecision(2) << maximo << std::endl;
    std::cout << "Promedio más bajo: " << std::fixed << std::setprecision(2) << minimo << std::endl;
    std::cout << "====================" << std::endl;
}

int main() {
    GestorEstudiantes gestor;
    int opcion;
    
    std::cout << "¡Bienvenido al Sistema de Gestión de Estudiantes!" << std::endl;
    std::cout << "Este sistema utiliza el algoritmo MergeSort para ordenar estudiantes por promedio ponderado." << std::endl;
    
    do {
        mostrarMenu();
        
        while (!(std::cin >> opcion) || opcion < 1 || opcion > 7) {
            std::cout << "Opción inválida. Por favor, seleccione una opción entre 1 y 7: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
        switch (opcion) {
            case 1: {
                gestor.ingresarEstudiantesDesdeConsola();
                break;
            }
            case 2: {
                gestor.mostrarEstudiantesNumerados();
                mostrarEstadisticas(gestor);
                break;
            }
            case 3: {
                gestor.ordenarPorPromedio();
                std::cout << "\nEstudiantes ordenados por promedio (menor a mayor):" << std::endl;
                gestor.mostrarEstudiantesNumerados();
                break;
            }
            case 4: {
                gestor.ordenarPorPromedioDescendente();
                std::cout << "\nEstudiantes ordenados por promedio (mayor a menor):" << std::endl;
                gestor.mostrarEstudiantesNumerados();
                break;
            }
            case 5: {
                agregarEstudianteIndividual(gestor);
                break;
            }
            case 6: {
                gestor.limpiarLista();
                break;
            }
            case 7: {
                std::cout << "¡Gracias por usar el Sistema de Gestión de Estudiantes!" << std::endl;
                std::cout << "¡Hasta luego!" << std::endl;
                break;
            }
        }
        
        if (opcion != 7) {
            std::cout << "\nPresione Enter para continuar...";
            std::cin.ignore();
            std::cin.get();
        }
        
    } while (opcion != 7);
    
    return 0;
}
