#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std;
using namespace chrono;

// Estructura para representar cada canción
struct Cancion {
    string id;       // Identificador único de la canción (Ej: SNG001)
    string nombre;   // Nombre o título de la canción
    double puntaje;  // Puntaje de la canción (para ordenarla)

    // Constructor para facilitar la creación de canciones
    Cancion(const string& _id, const string& _nombre, double _puntaje)
        : id(_id), nombre(_nombre), puntaje(_puntaje) {}
};

// Clase que maneja la carga, ordenamiento y exportación de canciones
class GestorCanciones {
private:
    vector<Cancion> listaCanciones;  // Lista de canciones
    int comparaciones;               // Contador de comparaciones
    int intercambios;                // Contador de intercambios
    long long tiempoEjecucion;       // Tiempo de ejecución en microsegundos

public:
    GestorCanciones() : comparaciones(0), intercambios(0), tiempoEjecucion(0) {}

    // Método para cargar las canciones desde un archivo
    bool cargarDesdeArchivo(const string& archivo) {
        ifstream inputFile(archivo);

        if (!inputFile.is_open()) {
            cerr << "No se pudo abrir el archivo " << archivo << endl;
            return false;
        }

        string linea;
        while (getline(inputFile, linea)) {
            if (linea.empty()) continue;  // Ignorar líneas vacías
            stringstream ss(linea);
            string id, nombre, puntajeStr;
            
            if (getline(ss, id, ',') && getline(ss, nombre, ',') && getline(ss, puntajeStr)) {
                try {
                    double puntaje = stod(puntajeStr);
                    listaCanciones.emplace_back(id, nombre, puntaje);
                } catch (const exception& e) {
                    cerr << "Error al leer la línea: " << linea << endl;
                }
            }
        }

        inputFile.close();
        return true;
    }

    // Método para mostrar la lista de canciones
    void mostrarCanciones() const {
        if (listaCanciones.empty()) {
            cout << "No hay canciones cargadas." << endl;
            return;
        }

        cout << "\n" << setw(10) << "ID" << setw(40) << "Nombre" << setw(10) << "Puntaje" << endl;
        cout << string(60, '-') << endl;
        for (const auto& cancion : listaCanciones) {
            cout << setw(10) << cancion.id
                 << setw(40) << cancion.nombre
                 << setw(10) << fixed << setprecision(1) << cancion.puntaje << endl;
        }
        cout << string(60, '-') << endl;
    }

    // Método para realizar el ordenamiento usando Insertion Sort (de mayor a menor)
    void ordenarPorPuntaje() {
        auto start = high_resolution_clock::now();  // Medir el tiempo de ejecución

        for (int i = 1; i < listaCanciones.size(); ++i) {
            Cancion clave = listaCanciones[i];
            int j = i - 1;
            while (j >= 0 && listaCanciones[j].puntaje < clave.puntaje) {
                listaCanciones[j + 1] = listaCanciones[j];
                j--;
                comparaciones++;
                intercambios++;
            }
            listaCanciones[j + 1] = clave;
            comparaciones++;
        }

        auto end = high_resolution_clock::now();
        tiempoEjecucion = duration_cast<microseconds>(end - start).count();
    }

    // Método para exportar la lista de canciones a un archivo CSV
    bool exportarACSV(const string& archivoSalida) {
        ofstream outputFile(archivoSalida);
        
        if (!outputFile.is_open()) {
            cerr << "No se pudo crear el archivo de salida " << archivoSalida << endl;
            return false;
        }

        outputFile << "ID,Nombre,Puntaje\n";
        for (const auto& cancion : listaCanciones) {
            outputFile << cancion.id << "," << "\"" << cancion.nombre << "\","
                       << fixed << setprecision(1) << cancion.puntaje << "\n";
        }
        outputFile.close();
        return true;
    }

    // Método para mostrar las estadísticas de la operación de ordenamiento
    void mostrarEstadisticas() const {
        cout << "\nEstadísticas del ordenamiento:" << endl;
        cout << "Canciones procesadas: " << listaCanciones.size() << endl;
        cout << "Tiempo de ejecución: " << tiempoEjecucion << " microsegundos" << endl;
        cout << "Comparaciones realizadas: " << comparaciones << endl;
        cout << "Intercambios realizados: " << intercambios << endl;
    }

    // Método que ejecuta todo el proceso: cargar, ordenar, mostrar y exportar
    void ejecutar() {
        cout << "Gestor de Canciones - Ordenamiento por Puntaje\n";
        cout << "==============================================\n";

        // Paso 1: Cargar canciones desde archivo
        if (!cargarDesdeArchivo("musica.txt")) {
            cerr << "No se pudo cargar el archivo. Asegúrate de que 'musica.txt' exista." << endl;
            return;
        }

        // Paso 2: Mostrar las canciones cargadas
        mostrarCanciones();

        // Paso 3: Ordenar las canciones
        ordenarPorPuntaje();

        // Paso 4: Mostrar las canciones ordenadas
        cout << "\nCanciones ordenadas por puntaje (mayor a menor):\n";
        mostrarCanciones();

        // Paso 5: Mostrar estadísticas
        mostrarEstadisticas();

        // Paso 6: Exportar las canciones ordenadas a un archivo CSV
        if (exportarACSV("canciones_ordenadas.csv")) {
            cout << "Archivo CSV exportado correctamente: canciones_ordenadas.csv" << endl;
        }
    }
};

int main() {
    GestorCanciones gestor;
    gestor.ejecutar();
    return 0;
}
