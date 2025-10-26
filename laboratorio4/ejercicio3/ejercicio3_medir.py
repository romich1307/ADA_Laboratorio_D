import random
import time
import heapq
import networkx as nx
import matplotlib.pyplot as plt
import os

# ---------- Utilidades ----------

def generar_grafo_conectado(n, m, seed=None):
    """Genera un grafo no dirigido conectado con n vértices y m aristas.
    Devuelve V (lista de vértices) y aristas (lista de tuplas (u,v,w)).
    """
    if seed is not None:
        random.seed(seed)
    if m < n - 1:
        raise ValueError("m debe ser al menos n-1 para conectar el grafo")
    max_aristas = n * (n - 1) // 2
    m = min(m, max_aristas)

    V = list(range(n))
    aristas = []
    disponibles = V.copy()
    random.shuffle(disponibles)
    # crear un árbol base
    for i in range(1, n):
        u = disponibles[i]
        v = random.choice(disponibles[:i])
        w = random.randint(1, 100)
        aristas.append((u, v, w))
    # añadir aristas adicionales
    while len(aristas) < m:
        u, v = random.sample(V, 2)
        if any(((a == u and b == v) or (a == v and b == u)) for (a, b, _) in aristas):
            continue
        w = random.randint(1, 100)
        aristas.append((u, v, w))
    return V, aristas


# Disjoint Set (Union-Find)
class DisjointSet:
    def __init__(self, n):
        self.parent = list(range(n))
        self.rank = [0] * n

    def find(self, x):
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]

    def union(self, x, y):
        rx = self.find(x)
        ry = self.find(y)
        if rx == ry:
            return False
        if self.rank[rx] < self.rank[ry]:
            self.parent[rx] = ry
        elif self.rank[ry] < self.rank[rx]:
            self.parent[ry] = rx
        else:
            self.parent[ry] = rx
            self.rank[rx] += 1
        return True


# Prim con heap (basado en ejercicio2/prim.py)
def prim_heap_mst(V, aristas, inicio=0):
    n = len(V)
    ady = {v: [] for v in V}
    for u, v, w in aristas:
        ady[u].append((v, w))
        ady[v].append((u, w))

    visitado = set()
    visitado.add(inicio)
    heap = []
    for vecino, peso in ady[inicio]:
        heapq.heappush(heap, (peso, inicio, vecino))

    mst = []
    pops = 0
    while heap and len(mst) < n - 1:
        peso, u, v = heapq.heappop(heap)
        pops += 1
        if v in visitado:
            continue
        mst.append((u, v, peso))
        visitado.add(v)
        for vecino, w in ady[v]:
            if vecino not in visitado:
                heapq.heappush(heap, (w, v, vecino))

    costo = sum(w for (_, _, w) in mst)
    return mst, costo, pops


# Kruskal (basado en ejercicio2/kruskal.py)
def kruskal_mst(V, aristas):
    ar_sorted = sorted(aristas, key=lambda x: x[2])
    dsu = DisjointSet(len(V))
    mst = []
    inspeccionadas = 0
    for u, v, w in ar_sorted:
        inspeccionadas += 1
        if dsu.find(u) != dsu.find(v):
            dsu.union(u, v)
            mst.append((u, v, w))
            if len(mst) == len(V) - 1:
                break
    costo = sum(w for (_, _, w) in mst)
    return mst, costo, inspeccionadas


def edges_set_normalized(mst):
    """Convierte lista de aristas a set de tuplas ordenadas (u,v,w) con u<=v para comparar."""
    return set(((min(a, b), max(a, b), w) for (a, b, w) in mst))


def dibujar_mst(V, aristas, mst, filename, title=None):
    G = nx.Graph()
    G.add_nodes_from(V)
    for u, v, w in aristas:
        G.add_edge(u, v, weight=w)
    pos = nx.spring_layout(G, seed=42)
    edge_labels = nx.get_edge_attributes(G, 'weight')

    plt.figure(figsize=(6, 5))
    if title:
        plt.title(title)
    nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=400)
    nx.draw_networkx_edge_labels(G, pos, edge_labels)
    # destacar MST
    mst_edges = [(a, b) for (a, b, _) in mst]
    nx.draw_networkx_edges(G, pos, edgelist=mst_edges, width=3, edge_color='green')
    plt.savefig(filename, dpi=150, bbox_inches='tight')
    plt.close()


def main():
    sizes = [10, 50, 100]
    outdir = os.path.join(os.path.dirname(__file__), 'salidas_ej3')
    os.makedirs(outdir, exist_ok=True)

    # elegimos m = min(n*(n-1)/2, 4*n) para tener grafos moderadamente densos
    for n in sizes:
        m = min(n * (n - 1) // 2, 4 * n)
        seed = 42 + n
        V, aristas = generar_grafo_conectado(n, m, seed=seed)
        print('\n' + '=' * 40)
        print(f'Grafo n={n}, m={m}, seed={seed}')

        # Kruskal
        t0 = time.perf_counter()
        mst_k, costo_k, inspeccionadas = kruskal_mst(V, aristas)
        t1 = time.perf_counter()
        tiempo_k = (t1 - t0)

        # Prim (heap)
        t2 = time.perf_counter()
        mst_p, costo_p, pops = prim_heap_mst(V, aristas, inicio=0)
        t3 = time.perf_counter()
        tiempo_p = (t3 - t2)

        print(f'Kruskal: costo={costo_k}, tiempo={tiempo_k:.6f}s, inspeccionadas={inspeccionadas}, aristas_mst={len(mst_k)}')
        print(f'Prim (heap): costo={costo_p}, tiempo={tiempo_p:.6f}s, pops={pops}, aristas_mst={len(mst_p)}')

        same = edges_set_normalized(mst_k) == edges_set_normalized(mst_p)
        print('¿Mismos conjuntos de aristas (considerando peso)?', 'Sí' if same else 'No')

        # Guardar gráficos
        base_name = f'ej3_n{n}'
        dibujar_mst(V, aristas, mst_k, os.path.join(outdir, base_name + '_kruskal_mst.png'), title=f'Kruskal MST n={n}')
        dibujar_mst(V, aristas, mst_p, os.path.join(outdir, base_name + '_prim_mst.png'), title=f'Prim MST n={n}')
        print('Gráficos guardados en', outdir)


if __name__ == '__main__':
    main()
