#ifndef ESTUDIANTE_H
#define ESTUDIANTE_H

#include <string>
#include <iostream>

class Estudiante {
private:
    std::string codigo;
    std::string nombre;
    float promedioPonderado;

public:
    // Constructores
    Estudiante();
    Estudiante(const std::string& codigo, const std::string& nombre, float promedio);
    
    // Getters
    std::string getCodigo() const;
    std::string getNombre() const;
    float getPromedioPonderado() const;
    
    // Setters
    void setCodigo(const std::string& codigo);
    void setNombre(const std::string& nombre);
    void setPromedioPonderado(float promedio);
    
    // Operadores
    bool operator<(const Estudiante& otro) const;
    bool operator>(const Estudiante& otro) const;
    bool operator<=(const Estudiante& otro) const;
    bool operator>=(const Estudiante& otro) const;
    
    // Función para mostrar información
    void mostrar() const;
    
    // Sobrecarga del operador <<
    friend std::ostream& operator<<(std::ostream& os, const Estudiante& estudiante);
};

#endif // ESTUDIANTE_H
