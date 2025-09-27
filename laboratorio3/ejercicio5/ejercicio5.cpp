#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

// -------- Modelo --------
struct Product {
    string code;
    string name;
    double price;
};

// -------- Util: split por coma (soporta tabs convirtiéndolos a comas) --------
static vector<string> splitCSV(string line) {
    for (char& c : line) if (c == '\t') c = ','; // tabs -> comas
    vector<string> out;
    string cur;
    stringstream ss(line);
    while (getline(ss, cur, ',')) out.push_back(cur);
    return out;
}

// -------- Lectura de archivo --------
static bool loadProducts(const string& path, vector<Product>& v) {
    ifstream in(path);
    if (!in) return false;
    string line;
    size_t lineno = 0;
    while (getline(in, line)) {
        ++lineno;
        // saltar líneas vacías
        if (line.find_first_not_of(" \t\r\n") == string::npos) continue;

        auto parts = splitCSV(line);
        if (parts.size() < 3) {
            cerr << "[Aviso] Linea " << lineno << ": formato invalido, se espera codigo,nombre,precio\n";
            continue;
        }
        // Si hay más de 3 partes, juntamos como nombre todo lo intermedio
        string code = parts.front();
        string priceStr = parts.back();
        string name;
        for (size_t i = 1; i + 1 < parts.size(); ++i) {
            if (i > 1) name += ",";
            name += parts[i];
        }
        // Trim básico
        auto trim = [](string& s){
            size_t a = s.find_first_not_of(" \t\r\n");
            size_t b = s.find_last_not_of(" \t\r\n");
            if (a == string::npos) { s.clear(); return; }
            s = s.substr(a, b - a + 1);
        };
        trim(code); trim(name); trim(priceStr);

        try {
            double price = stod(priceStr);
            v.push_back({code, name, price});
        } catch (...) {
            cerr << "[Aviso] Linea " << lineno << ": precio invalido -> \"" << priceStr << "\"\n";
        }
    }
    return true;
}

// =================== MERGESORT (desc por precio) ===================
static void mergeRange(vector<Product>& a, vector<Product>& aux, int l, int m, int r) {
    int i = l, j = m, k = l;
    while (i < m && j < r) {
        // mayor precio primero (desc). Estable con >= para lado izq en empates.
        if (a[i].price >= a[j].price) aux[k++] = a[i++];
        else                          aux[k++] = a[j++];
    }
    while (i < m) aux[k++] = a[i++];
    while (j < r) aux[k++] = a[j++];
    for (int t = l; t < r; ++t) a[t] = aux[t];
}
static void mergeSortRec(vector<Product>& a, vector<Product>& aux, int l, int r) {
    if (r - l <= 1) return;
    int m = l + (r - l) / 2;
    mergeSortRec(a, aux, l, m);
    mergeSortRec(a, aux, m, r);
    // early-exit: ya en desc si max izq >= min der
    if (a[m - 1].price >= a[m].price) return;
    mergeRange(a, aux, l, m, r);
}
static void mergeSortByPriceDesc(vector<Product>& a) {
    if (a.size() < 2) return;
    vector<Product> aux(a.size());
    mergeSortRec(a, aux, 0, (int)a.size());
}

// -------- Impresión --------
static void printProducts(const vector<Product>& v) {
    cout << left << setw(12) << "Codigo" << " | "
         << left << setw(32) << "Nombre" << " | "
         << right << setw(10) << "Precio\n";
    cout << string(12 + 3 + 32 + 3 + 10, '-') << "\n";
    cout << fixed << setprecision(2);
    for (const auto& p : v) {
        cout << left << setw(12) << p.code << " | "
             << left << setw(32) << p.name << " | "
             << right << setw(10) << p.price << "\n";
    }
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <archivo.txt>\n";
        cerr << "Formato: codigo,nombre,precio  (tambien soporta tab)\n";
        return 1;
    }

    vector<Product> items;
    if (!loadProducts(argv[1], items)) {
        cerr << "No se pudo abrir el archivo: " << argv[1] << "\n";
        return 1;
    }
    if (items.empty()) {
        cerr << "No se cargaron productos validos.\n";
        return 1;
    }

    // Ordenar DESC por precio con MergeSort
    mergeSortByPriceDesc(items);

    // Mostrar
    cout << "Productos ordenados (precio DESC) usando MergeSort:\n";
    printProducts(items);

    return 0;
}
