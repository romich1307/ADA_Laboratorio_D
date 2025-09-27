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

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);

    int n; cout << "n: ";
    if (!(cin >> n) || n < 0) { cerr << "invalido\n"; return 1; }

    int* a = new int[n];                     // arreglo
    cout << "Ingresa " << n << " enteros:\n";
    for (int i = 0; i < n; ++i) {
        while (!(cin >> a[i])) {             // valida
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Reintenta: ";
        }
    }

    if (n > 1) mergeSort(a, 0, n - 1);       // ordenar

    cout << "\nOrdenado: ";
    for (int i = 0; i < n; ++i)              // print
        cout << a[i] << (i + 1 < n ? ' ' : '\n');

    int x; cout << "\nBuscar: ";
    while (!(cin >> x)) {                    // valida
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Reintenta: ";
    }

    int idx = binarySearch(a, n, x);         // buscar
    if (idx != -1) cout << "Encontrado en " << idx << "\n";
    else           cout << "No encontrado\n";

    // Nota rápida: MergeSort O(n log n), Binary O(log n); total ~ O(n log n). Secuencial sin ordenar O(n).
    delete[] a; return 0;
}
