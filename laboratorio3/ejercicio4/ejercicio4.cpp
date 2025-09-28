#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace chrono;

// ---------- util: verificación ----------
bool isSorted(const vector<string>& a){
    for (size_t i=1;i<a.size();++i) if (a[i-1] > a[i]) return false; // asc
    return true;
}

// ---------- generador: 10k strings simples ----------
vector<string> makeRandomStrings(size_t n, int len=8){
    mt19937 rng(123456);                         // semilla fija (reproducible)
    uniform_int_distribution<int> d('a','z');   // letras minúsculas
    vector<string> v(n);
    for(size_t i=0;i<n;++i){
        string s; s.reserve(len);
        for(int k=0;k<len;++k) s.push_back(char(d(rng)));
        v[i]=std::move(s);
    }
    return v;
}

// ---------- Insertion Sort  ----------
void ordenamientoPorInsercion(vector<string>& lista){
    int tamaño = (int)lista.size();
    for (int i = 1; i < tamaño; i++) {
        string clave = lista[i];         // elemento a insertar
        int j = i - 1;
        while (j >= 0 && lista[j] > clave) { // desplaza mayores
            lista[j + 1] = lista[j];
            j--;
        }
        lista[j + 1] = clave;            // inserta en lugar correcto
    }
}

// ---------- MergeSort ----------
static void mergeRange(vector<string>& a, vector<string>& aux, int l, int m, int r){
    int i=l, j=m, k=l;                   // i: izq, j: der, k: escribe
    while(i<m && j<r) aux[k++] = (a[i] <= a[j]) ? a[i++] : a[j++]; // mezcla asc (estable con <=)
    while(i<m) aux[k++] = a[i++];        // restos izq
    while(j<r) aux[k++] = a[j++];        // restos der
    for(int t=l;t<r;++t) a[t] = aux[t];  // copia de vuelta
}
static void mergeSortRec(vector<string>& a, vector<string>& aux, int l, int r){
    if(r-l<=1) return;                   // tramo de 0/1 = ya ordenado
    int m = l + (r - l)/2;               // mitad
    mergeSortRec(a,aux,l,m);             // ordena izq
    mergeSortRec(a,aux,m,r);             // ordena der
    if(a[m-1] <= a[m]) return;        
    mergeRange(a,aux,l,m,r);             // mezcla
}
void mergeSort(vector<string>& a){
    if(a.size()<2) return;
    vector<string> aux(a.size());        // buffer único
    mergeSortRec(a,aux,0,(int)a.size());
}

// ---------- QuickSort ----------
static int particion(vector<string>& a, int bajo, int alto){
    string pivote = a[alto];             // último elemento como pivote
    int i = bajo - 1;                    // índice del menor
    for(int j = bajo; j < alto; j++){
        if(a[j] <= pivote){              // elemento menor o igual al pivote
            i++;
            swap(a[i], a[j]);            // intercambio
        }
    }
    swap(a[i+1], a[alto]);               // coloca pivote en posición correcta
    return i + 1;
}
static void quickSortRec(vector<string>& a, int bajo, int alto){
    if(bajo < alto){
        int pi = particion(a, bajo, alto); // índice de partición
        quickSortRec(a, bajo, pi - 1);     // ordena antes del pivote
        quickSortRec(a, pi + 1, alto);     // ordena después del pivote
    }
}
void quickSort(vector<string>& a){
    if(a.size()<2) return;
    quickSortRec(a, 0, (int)a.size()-1);
}

// ---------- Selection Sort ----------
void selectionSort(vector<string>& a){
    int n = (int)a.size();
    for(int i = 0; i < n-1; i++){
        int indiceMinimo = i;            // busca el mínimo
        for(int j = i+1; j < n; j++){
            if(a[j] < a[indiceMinimo]){
                indiceMinimo = j;
            }
        }
        if(indiceMinimo != i){           // intercambia si es necesario
            swap(a[i], a[indiceMinimo]);
        }
    }
}

// ---------- benchmark sencillo ----------
template<typename F>
pair<long long,bool> bench(const vector<string>& base, F sorter){
    vector<string> v = base;                         // misma entrada
    auto t0 = high_resolution_clock::now();
    sorter(v);
    auto t1 = high_resolution_clock::now();
    long long us = duration_cast<microseconds>(t1 - t0).count(); // microsegundos
    return {us, isSorted(v)};
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 10000;                             // 10,000 elementos
    auto base = makeRandomStrings(N);                // datos iguales para ambos

    auto [us_merge,  ok_merge ] = bench(base, [](auto& v){ mergeSort(v); });
    auto [us_quick,  ok_quick ] = bench(base, [](auto& v){ quickSort(v); });
    auto [us_insert, ok_insert] = bench(base, [](auto& v){ ordenamientoPorInsercion(v); });
    auto [us_select, ok_select] = bench(base, [](auto& v){ selectionSort(v); });

    cout << "n = " << N << " strings aleatorios\n\n";
    cout << left << setw(16) << "Algoritmo" << setw(14) << "Tiempo (us)" << "OK\n";
    cout << "---------------------------------------\n";
    cout << left << setw(16) << "MergeSort"     << setw(14) << us_merge  << (ok_merge ?"si":"NO")  << "\n";
    cout << left << setw(16) << "QuickSort"     << setw(14) << us_quick  << (ok_quick ?"si":"NO")  << "\n";
    cout << left << setw(16) << "InsertionSort" << setw(14) << us_insert << (ok_insert?"si":"NO")  << "\n";
    cout << left << setw(16) << "SelectionSort" << setw(14) << us_select << (ok_select?"si":"NO")  << "\n";
    return 0;
}
