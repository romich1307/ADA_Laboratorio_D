#include <bits/stdc++.h>
using namespace std;

// Programa: Optimización de Recursos para Gestión de Emergencias (Arequipa)
// Descripción: Calcula rutas eficientes entre provincias (MST con tiempos
// realistas) y optimiza la carga de vehículos usando el problema de la
// mochila (programación dinámica). Comentarios y variables en español.

struct Provincia {
    string nombre;
    double lat; // latitud (grados), aproximada
    double lon; // longitud (grados), aproximada
    double trafico; // factor multiplicativo >0, 1.0 = normal, >1 = más tráfico
    double condicion; // factor multiplicativo por estado de la vía, >=1
};

// Datos de equipos para la mochila
struct Equipo {
    string nombre;
    int peso; // unidades arbitrarias (por ejemplo kg o un índice de volumen)
    int valor; // importancia del equipo
};

// Constantes
const double PI = acos(-1.0);
const double RADIO_TIERRA_KM = 6371.0;

// Convierte grados a radianes
double aRadianes(double grados) {
    return grados * PI / 180.0;
}

// Distancia Haversine entre dos coordenadas (km)
double distanciaHaversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = aRadianes(lat2 - lat1);
    double dLon = aRadianes(lon2 - lon1);
    double rLat1 = aRadianes(lat1);
    double rLat2 = aRadianes(lat2);
    double a = sin(dLat/2)*sin(dLat/2) + cos(rLat1)*cos(rLat2)*sin(dLon/2)*sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    return RADIO_TIERRA_KM * c;
}

// Calcula tiempo en minutos estimado entre dos provincias
// Factores: distancia, velocidad media, multiplicadores de tráfico y condición
double tiempoEstimado(const Provincia &A, const Provincia &B) {
    double distancia_km = distanciaHaversine(A.lat, A.lon, B.lat, B.lon);
    double velocidad_media_kmh = 50.0; // velocidad media asumiendo caminos mixtos
    double tiempo_horas = distancia_km / velocidad_media_kmh;
    double tiempo_min = tiempo_horas * 60.0;
    // aplicar factores realistas (tráfico + condiciones) y añadir tiempo fijo de maniobra
    double factor = (A.trafico + B.trafico) / 2.0; // promedio de tráfico entre nodos
    double cond = (A.condicion + B.condicion) / 2.0; // promedio condición vial
    double tiempo_total = tiempo_min * factor * cond + 10.0; // +10 min carga/descarga
    return tiempo_total;
}

// Prim para obtener MST en grafo completo con pesos = tiempo en minutos
vector<pair<int,int>> primMST(const vector<vector<double>> &matrizPesos) {
    int n = matrizPesos.size();
    vector<double> key(n, 1e18);
    vector<int> parent(n, -1);
    vector<bool> inMST(n, false);
    key[0] = 0.0;
    for (int count=0; count<n-1; ++count) {
        // encontrar vértice mínimo no incluido
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

// Estructura y funciones para Kruskal (Union-Find)
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

// Mochila 0/1 (DP) que devuelve la lista de índices seleccionados
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
    // reconstruir solución
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
    // 1) Definir provincias (coordenadas aproximadas) y factores
    vector<Provincia> provincias = {
        // nombre, lat, lon, trafico, condicion
        {"Arequipa", -16.409047, -71.537451, 1.25, 1.10},
        {"Camaná",   -16.641142, -72.128143, 1.10, 1.05},
        {"Islay",    -17.013091, -72.013061, 1.00, 1.00},
        {"Caravelí", -15.766667, -74.196111, 1.30, 1.20},
        {"La Unión", -15.640000, -72.200000, 1.40, 1.30},
        {"Condesuyos",-15.680000, -72.560000,1.35, 1.25},
        {"Castilla", -16.500000, -72.000000, 1.20, 1.15},
        {"Caylloma", -15.633333, -71.616667, 1.20, 1.10}
    };

    int n = provincias.size();

    // 2) Construir matriz de tiempos (en minutos) entre todas las provincias
    // Ofrecemos dos modos:
    // - modo 1 (completo): grafo completo con tiempo calculado por Haversine
    // - modo 2 (realista): grafo con conectividad manual (algunas aristas directas)
    cout << "Elija modo de grafo (1=completo, 2=realista con conectividad) [Por defecto 2]: ";
    int modoGrafo = 2;
    if(!(cin >> modoGrafo)) modoGrafo = 2;

    const double INF = 1e12;
    vector<vector<double>> matrizTiempo(n, vector<double>(n, INF));
    // definimos conectividad manual (simétrica) para el "grafo realista".
    // true = existe carretera directa entre provincias (aprox). Ajuste según datos reales.
    vector<vector<int>> conectividad(n, vector<int>(n, 0));
    // Índices: 0=Arequipa,1=Camaná,2=Islay,3=Caravelí,4=La Unión,5=Condesuyos,6=Castilla,7=Caylloma
    // Conectividad aproximada (asegura que el grafo sea conexo)
    conectividad[0][6] = conectividad[6][0] = 1; // Arequipa - Castilla
    conectividad[0][7] = conectividad[7][0] = 1; // Arequipa - Caylloma
    conectividad[0][1] = conectividad[1][0] = 1; // Arequipa - Camaná
    conectividad[1][2] = conectividad[2][1] = 1; // Camaná - Islay
    conectividad[6][1] = conectividad[1][6] = 1; // Castilla - Camaná
    conectividad[7][4] = conectividad[4][7] = 1; // Caylloma - La Unión
    conectividad[4][5] = conectividad[5][4] = 1; // La Unión - Condesuyos
    conectividad[5][3] = conectividad[3][5] = 1; // Condesuyos - Caravelí (ruta larga)
    conectividad[6][5] = conectividad[5][6] = 1; // Castilla - Condesuyos
    // Para el modo completo, marcamos todas como conectadas
    if (modoGrafo == 1) {
        for (int i=0;i<n;++i) for (int j=0;j<n;++j) if (i!=j) conectividad[i][j]=1;
        cout << "(Usando grafo completo basado en distancia Haversine)\n";
    } else {
        cout << "(Usando grafo realista con conectividad manual)\n";
    }

    for (int i=0; i<n; ++i) {
        matrizTiempo[i][i] = 0.0;
        for (int j=i+1; j<n; ++j) {
            if (conectividad[i][j]) {
                // usar distancia realista: Haversine * factor carretero
                double distancia_km = distanciaHaversine(provincias[i].lat, provincias[i].lon,
                                                         provincias[j].lat, provincias[j].lon);
                double factor_carretera = 1.25; // asume rutas más largas por curvas/terreno
                double distancia_road_km = distancia_km * factor_carretera;
                // crear provincias temporales para calcular tiempo (usando los factores de trafico/condicion)
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

    // 3) Calcular MST: permitir elegir Prim o Kruskal.
    cout << "Elija algoritmo MST (1=Prim, 2=Kruskal) [Por defecto 1]: ";
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
    cout << "--- Rutas (Árbol de Expansión Mínima - MST) para Arequipa (tiempos en minutos) ---\n";
    double sumaTiempos = 0.0;
    for (auto &e : aristasMST) {
        int u = e.first, v = e.second;
        double t = matrizTiempo[u][v];
        cout << provincias[u].nombre << " <-> " << provincias[v].nombre << ": " << t << " minutos\n";
        sumaTiempos += t;
    }
    cout << "Tiempo total aproximado del MST (suma aristas): " << sumaTiempos << " minutos\n\n";

    // 4) Definir equipos disponibles (peso y valor)
    vector<Equipo> equipos = {
        {"Camilla", 20, 90},
        {"Botiquin completo", 15, 80},
        {"Ropa de abrigo (paquete)", 10, 40},
        {"Agua (cajas)", 25, 60},
        {"Comida (raciones)", 30, 70},
        {"Herramientas (rescate)", 40, 85},
        {"Generador pequeño", 60, 95},
        {"Linternas/Radio", 5, 30},
        {"Kit de comunicaciones", 12, 65}
    };

    // 5) Definir 3 tipos de vehículos con capacidades
    vector<pair<string,int>> vehiculos = {
        {"Camioneta ligera", 100},
        {"Camion mediano", 200},
        {"Camion grande", 350}
    };

    cout << "--- Optimización de carga por vehículo (Mochila 0/1 DP) ---\n";
    for (auto &veh : vehiculos) {
        string nombreVeh = veh.first;
        int capacidad = veh.second;
        vector<int> seleccion = mochilaknapSack(equipos, capacidad);
        // Calcular peso y valor total seleccionado
        int pesoTotal = 0, valorTotal = 0;
        cout << "\nVehículo: " << nombreVeh << " (capacidad " << capacidad << ")\n";
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
