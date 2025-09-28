#include <iostream>
#include <limits>
using namespace std;

// merge: mezcla [l..m] y [m+1..r] (asc)
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;          // tamaños
    int* L = new int[n1];                    // temp izq
    int* R = new int[n2];                    // temp der
    for (int i = 0; i < n1; ++i) L[i] = arr[l + i];     // copia izq
    for (int j = 0; j < n2; ++j) R[j] = arr[m + 1 + j]; // copia der
    int i = 0, j = 0, k = l;                 // punteros
    while (i < n1 && j < n2)                 // fusion
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];        // restos izq
    while (j < n2) arr[k++] = R[j++];        // restos der
    delete[] L; delete[] R;                  // liberar
}

// mergeSort: divide y mezcla (asc)
void mergeSort(int arr[], int l, int r) {
    if (l >= r) return;                      // caso base
    int m = l + (r - l) / 2;                 // mitad
    mergeSort(arr, l, m);                    // izq
    mergeSort(arr, m + 1, r);                // der
    if (arr[m] <= arr[m + 1]) return;        // early exit
    merge(arr, l, m, r);                     // mezcla
}

// partition: partición para quickSort
int partition(int arr[], int low, int high) {
    int pivot = arr[high];                   // pivote
    int i = low - 1;                         // índice menor
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);            // intercambio
        }
    }
    swap(arr[i + 1], arr[high]);             // pivote en posición
    return i + 1;
}

// quickSort: ordena por división
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);  // partición
        quickSort(arr, low, pi - 1);         // izq
        quickSort(arr, pi + 1, high);        // der
    }
}

// binarySearch: busca target en arr asc
int binarySearch(const int arr[], int n, int target) {
    int lo = 0, hi = n - 1;                  // rangos
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;        // centro
        if (arr[mid] == target) return mid;  // hallado
        if (arr[mid] < target) lo = mid + 1; // derecha
        else hi = mid - 1;                   // izquierda
    }
    return -1;                               // no está
}

// sequentialSearch: busca target en arr original (sin ordenar)
int sequentialSearch(const int arr[], int n, int target) {
    for (int i = 0; i < n; ++i) {
        if (arr[i] == target) return i;      // hallado
    }
    return -1;                               // no está
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);

    cout << "=== ORDENAMIENTO Y BUSQUEDA ===" << endl;
    cout << "MergeSort/QuickSort + Busqueda Binaria vs Busqueda Secuencial" << endl;
    cout << "===============================================" << endl << endl;
    
    int n;
    cout << "Ingrese el tamano del arreglo (n): ";
    cout.flush(); 
    if (!(cin >> n) || n < 0) { cerr << "Numero invalido\n"; return 1; }

    int* a = new int[n];                     // arreglo
    int* original = new int[n];              // copia original
    cout << "Ingresa " << n << " enteros:\n";
    cout.flush(); 
    for (int i = 0; i < n; ++i) {
        while (!(cin >> a[i])) {             // valida
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Reintenta: ";
        }
        original[i] = a[i];                  // guarda original
    }

    // Elegir algoritmo de ordenamiento
    int choice;
    cout << "\n--- SELECCION DE ALGORITMO ---" << endl;
    cout << "1. MergeSort  " << endl;
    cout << "2. QuickSort  " << endl;
    cout << "Elija opcion (1 o 2): ";
    cout.flush(); // Forzar salida inmediata
    cin >> choice;

    if (choice == 1) {
        if (n > 1) mergeSort(a, 0, n - 1);       // ordenar con merge
        cout << "Aplicando MergeSort..." << endl;
    } else {
        if (n > 1) quickSort(a, 0, n - 1);      // ordenar con quick
        cout << "Aplicando QuickSort..." << endl;
    }

    cout << "\n--- ARREGLO ORDENADO ---" << endl;
    cout << "Resultado: ";
    for (int i = 0; i < n; ++i)              // print
        cout << a[i] << (i + 1 < n ? ' ' : '\n');

    int x; cout << "\n--- BUSQUEDA ---" << endl;
    cout << "Ingrese elemento a buscar: ";
    cout.flush(); // Forzar salida inmediata
    while (!(cin >> x)) {                    // valida
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Reintenta: ";
    }

    cout << "\n--- RESULTADOS ---" << endl;
    int idx = binarySearch(a, n, x);         // buscar
    cout << "Busqueda Binaria (en arreglo ordenado): ";
    if (idx != -1) cout << "Encontrado en posicion " << idx << endl;
    else           cout << "No encontrado" << endl;

    // Comparación con búsqueda secuencial
    int seqIdx = sequentialSearch(original, n, x);
    cout << "Busqueda Secuencial (arreglo original): ";
    if (seqIdx != -1) cout << "Encontrado en posicion " << seqIdx << endl;
    else              cout << "No encontrado" << endl;

    delete[] a; delete[] original; return 0;
}
