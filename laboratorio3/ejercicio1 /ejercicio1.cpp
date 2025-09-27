#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
using namespace std;

// =================== Modelo ===================
// Estructura simple para no complicarnos.
// code y name como strings, y gpa en double (promedio).
struct Student {
    string code;
    string name;
    double gpa; // Promedio ponderado
};

// =================== merge ===================
// Mezcla dos subarreglos ya ordenados: [l..m] y [m+1..r].
// Orden ascendente por gpa y **ESTABLE**.
// Nota: uso new[]/delete[] porque los VLAs (int L[n]) no son estándar en C++.
void merge(Student arr[], int l, int m, int r) {
    int n1 = m - l + 1;   // tamaño lado izquierdo
    int n2 = r - m;       // tamaño lado derecho

    // Buffers temporales (sí, aquí hay dos new; luego se liberan).
    Student* L = new Student[n1];
    Student* R = new Student[n2];

    // Copio los dos tramos
    for (int i = 0; i < n1; ++i) L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    // Fusión clásica.
    // Detalle importante: uso <= para que, si empatan en GPA,
    // salga primero el del lado izquierdo -> esto hace el algoritmo ESTABLE.
    while (i < n1 && j < n2) {
        if (L[i].gpa <= R[j].gpa) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    // Por si sobran elementos de alguno de los lados
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    // Limpieza
    delete[] L;
    delete[] R;
}

// =================== mergeSort ===================
// Divide y vencerás: ordeno [l..r] con recursión.
// **Mejora**: antes de llamar a merge, reviso si ya está ordenado
// (arr[m].gpa <= arr[m+1].gpa). Si sí, me salto la fusión y ahorro trabajo.
void mergeSort(Student arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        // Ordeno cada mitad
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // --- Mejora (early exit) ---
        // Si el mayor de la izquierda <= el menor de la derecha,
        // entonces todo el tramo ya está OK; no necesito mezclar.
        if (arr[m].gpa <= arr[m + 1].gpa) return;

        // Si no, hago la fusión normal.
        merge(arr, l, m, r);
    }
}

// =================== main ===================
// Leo N estudiantes, ordeno por GPA ascendente (estable) y muestro.
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cout << "Cantidad de estudiantes: ";
    if (!(cin >> n) || n < 0) {
        cerr << "Entrada invalida.\n";
        return 1;
    }
    // Limpio el salto de linea pendiente para que getline funcione bien.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Arreglo dinámico (simple y directo, como en clase).
    Student* a = new Student[n];

    cout << "\nIngrese Codigo, Nombre y Promedio (ej. 15.75)\n";
    for (int i = 0; i < n; ++i) {
        cout << "\nEstudiante #" << (i + 1) << "\n";

        cout << "Codigo: ";
        getline(cin, a[i].code);
        // Evito códigos vacíos (cosita básica de validación).
        while (a[i].code.empty()) {
            cout << "Codigo no puede estar vacio. Intente: ";
            getline(cin, a[i].code);
        }

        cout << "Nombre: ";
        getline(cin, a[i].name);

        cout << "Promedio: ";
        while (!(cin >> a[i].gpa)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Promedio invalido. Intente (ej. 14.25): ";
        }
        // Limpio buffer para el siguiente getline
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Ordeno si hay algo que ordenar
    if (n > 0) mergeSort(a, 0, n - 1);

    // Muestro salida bonita con 2 decimales
    cout << "\n=== Lista ordenada por Promedio (ascendente) ===\n";
    for (int i = 0; i < n; ++i) {
        cout << a[i].code << " | " << a[i].name
             << " | GPA: " << fixed << setprecision(2) << a[i].gpa << "\n";
    }

    delete[] a; // liberar la memoria del arreglo principal
    return 0;
}
