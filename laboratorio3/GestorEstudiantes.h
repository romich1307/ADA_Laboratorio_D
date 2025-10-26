#ifndef GESTORESTUDIANTES_H
#define GESTORESTUDIANTES_H

#include "Estudiante.h"
#include <vector>

class GestorEstudiantes {
private:
    std::vector<Estudiante> estudiantes;
    
public:
    // Constructor
    GestorEstudiantes();
    
    // Funciones para gestionar estudiantes
    void agregarEstudiante(const Estudiante& estudiante);
    void ingresarEstudiantesDesdeConsola();
    void mostrarEstudiantes() const;
    void mostrarEstudiantesNumerados() const;
    
    // Funciones de ordenamiento
    void ordenarPorPromedio();
    void ordenarPorPromedioDescendente();
    
    // Getters
    std::vector<Estudiante>& getEstudiantes();
    const std::vector<Estudiante>& getEstudiantes() const;
    int getCantidadEstudiantes() const;
    
    // Función para limpiar la lista
    void limpiarLista();
};

#endif // GESTORESTUDIANTES_H
