#include <bits/stdc++.h>
using namespace std;

// Programa: Optimizacion de Recursos para Gestion de Emergencias (Arequipa)
// Descripcion: Implemento herramientas para calcular rutas eficientes entre
// provincias y estimar tiempos de desplazamiento realistas. Ademas optimizo
// la carga de vehiculos mediante el problema de la mochila 0/1 (programacion
// dinamica). Comentarios y nombres estan en espanol para facilitar su lectura.

struct Provincia {
    string nombre;
    double lat;              // latitud (grados), aproximada
    double lon;              // longitud (grados), aproximada
    double trafico;          // factor multiplicativo >0, 1.0 = normal, >1 = mas trafico
    double condicion;        // factor multiplicativo por estado de la via, >=1
};

// Datos de equipos para la mochila (lista de items que puedo elegir para cargar)
struct Equipo {
    string nombre;
    int peso; // unidades arbitrarias (por ejemplo kg o un indice de volumen)
    int valor; // importancia del equipo
};

// Constantes geometricas y de referencia (las utilizo en Haversine)
const double PI = acos(-1.0);
const double RADIO_TIERRA_KM = 6371.0;

// Convierto grados a radianes: paso de unidades geograficas a radianes
// porque las funciones trigonometricas de C++ trabajan en radianes.
double aRadianes(double grados) {
    return grados * PI / 180.0;
}

// Distancia Haversine entre dos coordenadas (km)
// Uso la formula de Haversine para obtener la distancia sobre la superficie
// terrestre entre dos puntos dados por latitud/longitud. Devuelve kilometros.
double distanciaHaversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = aRadianes(lat2 - lat1);
    double dLon = aRadianes(lon2 - lon1);
    double rLat1 = aRadianes(lat1);
    double rLat2 = aRadianes(lat2);
    double a = sin(dLat/2)*sin(dLat/2) + cos(rLat1)*cos(rLat2)*sin(dLon/2)*sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    return RADIO_TIERRA_KM * c;
}

// Calculo un tiempo estimado (minutos) entre dos provincias
// Considero: distancia entre puntos (Haversine), velocidad media de la via,
// factores multiplicativos de trafico y condicion de la carretera, y un
// tiempo fijo de maniobra (carga/descarga). Devuelvo minutos aproximados.
double tiempoEstimado(const Provincia A, const Provincia B) {
    double distancia_km = distanciaHaversine(A.lat, A.lon, B.lat, B.lon);
    double velocidad_media_kmh = 50.0;
    double tiempo_horas = distancia_km / velocidad_media_kmh;
    double tiempo_min = tiempo_horas * 60.0;
                                                                                                                     // aplicar factores realistas (trafico + condiciones) y anadir tiempo fijo de maniobra
    double factor = (A.trafico + B.trafico) / 2.0;                                  // promedio de trafico entre nodos
    double cond = (A.condicion + B.condicion) / 2.0;                            // promedio condicion vial
    double tiempo_total = tiempo_min * factor * cond + 10.0;                        // +10 min carga/descarga
    return tiempo_total;
}

// Implemento Prim (version O(n^2)) para obtener el arbol de expansion minima
// (MST). Aqui asumo una matriz de pesos completa y uso una busqueda lineal
// del vertice con clave minima en cada iteracion — adecuado para n pequeno.
vector<pair<int,int>> primMST(const vector<vector<double>> &matrizPesos) {
    int n = matrizPesos.size();
    vector<double> key(n, 1e18);
    vector<int> parent(n, -1);
    vector<bool> inMST(n, false);
    key[0] = 0.0;
    for (int count=0; count<n-1; ++count) {
        // encontrar vertice minimo no incluido
        double minv = 1e18; int u = -1;
        for (int v=0; v<n; ++v) if (!inMST[v] && key[v] < minv) { minv = key[v]; u = v; }
        if (u == -1) break;
        inMST[u] = true;
        for (int v=0; v<n; ++v) {
            if (!inMST[v] && matrizPesos[u][v] < key[v]) {
                key[v] = matrizPesos[u][v];
                parent[v] = u;
            }
        }
    }
    vector<pair<int,int>> aristas;
    for (int i=1; i<n; ++i) if (parent[i] != -1) aristas.push_back({parent[i], i});
    return aristas;
}

// Implemento Union-Find (disjoint set) y Kruskal para obtener el MST.
// Union-Find me permite unir componentes y detectar ciclos de forma eficiente.
struct UnionFind {
    int n;
    vector<int> p, r;
    UnionFind(int n): n(n), p(n), r(n,0) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x]==x?x:p[x]=find(p[x]); }
    bool unite(int a,int b){
        a=find(a); b=find(b); if(a==b) return false;
        if(r[a]<r[b]) swap(a,b);
        p[b]=a; if(r[a]==r[b]) r[a]++;
        return true;
    }
};

// Kruskal: construyo una lista de aristas (u<v), las ordeno por peso y las
// anado si conectan componentes diferentes usando Union-Find. Ideal para
// grafos esparsos; para grafos densos Prim (si esta optimizado) suele ser mas rapido.
vector<pair<int,int>> kruskalMST(const vector<vector<double>> &matrizPesos) {
    int n = matrizPesos.size();
    struct Edge{double w; int u,v;};
    vector<Edge> edges;
    for(int i=0;i<n;++i) for(int j=i+1;j<n;++j) edges.push_back({matrizPesos[i][j], i, j});
    sort(edges.begin(), edges.end(), [](const Edge&a,const Edge&b){return a.w<b.w;});
    UnionFind uf(n);
    vector<pair<int,int>> aristas;
    for(auto &e: edges){
        if(uf.unite(e.u, e.v)){
            aristas.push_back({e.u, e.v});
            if((int)aristas.size() == n-1) break;
        }
    }
    return aristas;
}

// Mochila 0/1 por programacion dinamica.
// Construyo la tabla `dp[i][w]` que representa el valor maximo usando los
// primeros i items con capacidad w. Al final reconstruyo los indices
// seleccionados retrocediendo por la tabla.
vector<int> mochilaknapSack(const vector<Equipo> &items, int capacidad) {
    int n = items.size();
    vector<vector<int>> dp(n+1, vector<int>(capacidad+1, 0));
    for (int i=1; i<=n; ++i) {
        for (int w=0; w<=capacidad; ++w) {
            dp[i][w] = dp[i-1][w];
            if (w >= items[i-1].peso) {
                dp[i][w] = max(dp[i][w], dp[i-1][w - items[i-1].peso] + items[i-1].valor);
            }
        }
    }
    // reconstruir solucion
    int w = capacidad;
    vector<int> seleccion;
    for (int i=n; i>=1; --i) {
        if (dp[i][w] != dp[i-1][w]) {
            seleccion.push_back(i-1); // incluimos el item i-1
            w -= items[i-1].peso;
        }
    }
    reverse(seleccion.begin(), seleccion.end());
    return seleccion;
}

int main() {
    // 1) Defino provincias (coordenadas aproximadas) y factores
    vector<Provincia> provincias = {
        // nombre, latitud, longitud, trafico, condicion
        {"Arequipa", -16.409047, -71.537451, 1.25, 1.10},
        {"Camana",   -16.641142, -72.128143, 1.10, 1.05},
        {"Islay",    -17.013091, -72.013061, 1.00, 1.00},
        {"Caraveli", -15.766667, -74.196111, 1.30, 1.20},
        {"La Union", -15.640000, -72.200000, 1.40, 1.30},
        {"Condesuyos",-15.680000, -72.560000,1.35, 1.25},
        {"Castilla", -16.500000, -72.000000, 1.20, 1.15},
        {"Caylloma", -15.633333, -71.616667, 1.20, 1.10}
    };

    int n = provincias.size();

    // 2) Construyo la matriz de tiempos (minutos) entre todas las provincias.
    // Uso el modo realista: una conectividad manual que representa carreteras
    // directas aproximadas entre provincias (solo esas parejas tendran arista).
    const double INF = 1e12;
    vector<vector<double>> matrizTiempo(n, vector<double>(n, INF));
    // Defino la conectividad manual (simetrica) para el "grafo realista".
    // Un valor 1 indica que estimo una carretera directa entre esas provincias.
    // Estos valores son aproximados y se pueden ajustar con datos reales.
    vector<vector<int>> conectividad(n, vector<int>(n, 0));
    // Indices: 0=Arequipa,1=Camana,2=Islay,3=Caraveli,4=La Union,5=Condesuyos,6=Castilla,7=Caylloma
    // Conectividad aproximada (asegura que el grafo sea conexo)
    conectividad[0][6] = conectividad[6][0] = 1; // Arequipa - Castilla
    conectividad[0][7] = conectividad[7][0] = 1; // Arequipa - Caylloma
    conectividad[0][1] = conectividad[1][0] = 1; // Arequipa - Camana
    conectividad[1][2] = conectividad[2][1] = 1; // Camana - Islay
    conectividad[6][1] = conectividad[1][6] = 1; // Castilla - Camana
    conectividad[7][4] = conectividad[4][7] = 1; // Caylloma - La Union
    conectividad[4][5] = conectividad[5][4] = 1; // La Union - Condesuyos
    conectividad[5][3] = conectividad[3][5] = 1; // Condesuyos - Caraveli (ruta larga)
    conectividad[6][5] = conectividad[5][6] = 1; // Castilla - Condesuyos
    // (modo realista): la conectividad ya esta definida manualmente arriba

    for (int i=0; i<n; ++i) {
        matrizTiempo[i][i] = 0.0;
        for (int j=i+1; j<n; ++j) {
            if (conectividad[i][j]) {
                // Calculo una distancia de carretera aproximada: Haversine * factor carretero
                double distancia_km = distanciaHaversine(provincias[i].lat, provincias[i].lon,
                                                         provincias[j].lat, provincias[j].lon);
                double factor_carretera = 1.25; // asume rutas mas largas por curvas/terreno
                double distancia_road_km = distancia_km * factor_carretera;
                // Uso los factores de trafico y condicion de las provincias para ajustar el tiempo.
                Provincia A = provincias[i]; Provincia B = provincias[j];
                // velocidad media en km/h adaptada a distancia carretera
                double velocidad_media_kmh = 50.0;
                double tiempo_min = (distancia_road_km / velocidad_media_kmh) * 60.0;
                double factor = (A.trafico + B.trafico) / 2.0;
                double cond = (A.condicion + B.condicion) / 2.0;
                double tiempo_total = tiempo_min * factor * cond + 10.0; // +10 min maniobra
                matrizTiempo[i][j] = matrizTiempo[j][i] = tiempo_total;
            } else {
                matrizTiempo[i][j] = matrizTiempo[j][i] = INF; // no hay arista directa
            }
        }
    }

    // 3) Calculo el MST: permito elegir entre Prim o Kruskal segun prefiera.
    cout << "Elija algoritmo MST (1=Prim, 2=Kruskal): ";
    int opcion = 1;
    if(!(cin >> opcion)) opcion = 1;
    vector<pair<int,int>> aristasMST;
    if (opcion == 2) {
        aristasMST = kruskalMST(matrizTiempo);
        cout << "(Usando Kruskal)\n";
    } else {
        aristasMST = primMST(matrizTiempo);
        cout << "(Usando Prim)\n";
    }

    cout << fixed << setprecision(2);
    cout << "--- Rutas (Arbol de Expansion Minima - MST) para Arequipa (tiempos en minutos) ---\n";
    double sumaTiempos = 0.0;
    for (auto &e : aristasMST) {
        int u = e.first, v = e.second;
        double t = matrizTiempo[u][v];
        cout << provincias[u].nombre << " <-> " << provincias[v].nombre << ": " << t << " minutos\n";
        sumaTiempos += t;
    }
    cout << "Tiempo total aproximado del MST (suma aristas): " << sumaTiempos << " minutos\n\n";

    // 4) Defino los equipos disponibles (peso en unidades arbitrarias y valor)
    vector<Equipo> equipos = {
        {"Camilla", 20, 90},
        {"Botiquin completo", 15, 80},
        {"Ropa de abrigo (paquete)", 10, 40},
        {"Agua (cajas)", 25, 60},
        {"Comida (raciones)", 30, 70},
        {"Herramientas (rescate)", 40, 85},
        {"Generador pequeno", 60, 95},
        {"Linternas/Radio", 5, 30},
        {"Kit de comunicaciones", 12, 65}
    };

    // 5) Defino 3 tipos de vehiculos con su capacidad maxima (peso) y aplico
    //    la optimizacion de carga para cada vehiculo.
    vector<pair<string,int>> vehiculos = {
        {"Camioneta pequena", 100},
        {"Camion mediano", 200},
        {"Camion grande", 350}
    };

    cout << "--- Optimizacion de carga por vehiculo (Mochila 0/1 DP) ---\n";
    for (auto &veh : vehiculos) {
        string nombreVeh = veh.first;
        int capacidad = veh.second;
        vector<int> seleccion = mochilaknapSack(equipos, capacidad);
        // Calcular peso y valor total seleccionado
        int pesoTotal = 0, valorTotal = 0;
        cout << "\nVehiculo: " << nombreVeh << " (capacidad " << capacidad << ")\n";
        cout << "Equipos seleccionados:\n";
        for (int idx : seleccion) {
            cout << " - " << equipos[idx].nombre << " (peso=" << equipos[idx].peso << ", valor=" << equipos[idx].valor << ")\n";
            pesoTotal += equipos[idx].peso;
            valorTotal += equipos[idx].valor;
        }
        cout << "Peso total: " << pesoTotal << " / " << capacidad << "\n";
        cout << "Valor total optimizado: " << valorTotal << "\n";
    }
    return 0;
}
