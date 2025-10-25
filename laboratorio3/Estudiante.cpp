#include "Estudiante.h"
#include <iomanip>

// Constructores
Estudiante::Estudiante() : codigo(""), nombre(""), promedioPonderado(0.0f) {}

Estudiante::Estudiante(const std::string& codigo, const std::string& nombre, float promedio)
    : codigo(codigo), nombre(nombre), promedioPonderado(promedio) {}

// Getters
std::string Estudiante::getCodigo() const {
    return codigo;
}

std::string Estudiante::getNombre() const {
    return nombre;
}

float Estudiante::getPromedioPonderado() const {
    return promedioPonderado;
}

// Setters
void Estudiante::setCodigo(const std::string& codigo) {
    this->codigo = codigo;
}

void Estudiante::setNombre(const std::string& nombre) {
    this->nombre = nombre;
}

void Estudiante::setPromedioPonderado(float promedio) {
    this->promedioPonderado = promedio;
}

// Operadores de comparación (basados en promedio ponderado)
bool Estudiante::operator<(const Estudiante& otro) const {
    return this->promedioPonderado < otro.promedioPonderado;
}

bool Estudiante::operator>(const Estudiante& otro) const {
    return this->promedioPonderado > otro.promedioPonderado;
}

bool Estudiante::operator<=(const Estudiante& otro) const {
    return this->promedioPonderado <= otro.promedioPonderado;
}

bool Estudiante::operator>=(const Estudiante& otro) const {
    return this->promedioPonderado >= otro.promedioPonderado;
}

// Función para mostrar información del estudiante
void Estudiante::mostrar() const {
    std::cout << "Código: " << codigo 
              << " | Nombre: " << nombre 
              << " | Promedio: " << std::fixed << std::setprecision(2) << promedioPonderado << std::endl;
}

// Sobrecarga del operador << para facilitar la impresión
std::ostream& operator<<(std::ostream& os, const Estudiante& estudiante) {
    os << "Código: " << estudiante.codigo 
       << " | Nombre: " << estudiante.nombre 
       << " | Promedio: " << std::fixed << std::setprecision(2) << estudiante.promedioPonderado;
    return os;
}
