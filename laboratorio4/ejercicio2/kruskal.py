from functools import cmp_to_key
from ciclos import Ciclos
import networkx as nx
import matplotlib.pyplot as plt

def comparador(a, b):
    return a[2] - b[2]

def kruskals_mst(V, aristas, mostrar_graficos=False):
    """
    Aplica el algoritmo de Kruskal y opcionalmente muestra el proceso paso a paso.
    - V: lista de vértices
    - aristas: lista de tuplas (u, v, peso)
    - mostrar_graficos: si True, dibuja los pasos del MST
    """
    # ordenar aristas por peso
    aristas = sorted(aristas, key=cmp_to_key(comparador))

    dsu = Ciclos(len(V))
    costo_total = 0
    mst = []           # lista de aristas que van al MST
    pasos = []         # lista de pasos (para graficar)

    for u, v, peso in aristas:
        # verificar si los nodos están en distintos conjuntos
        if dsu.encontrar(u) != dsu.encontrar(v):
            dsu.union(u, v)
            mst.append((u, v, peso))
            costo_total += peso
            pasos.append((mst.copy(), (u, v, peso), "agregado"))
        else:
            pasos.append((mst.copy(), (u, v, peso), "descartado (ciclo)"))

        # si ya se tienen n−1 aristas, se termina
        if len(mst) == len(V) - 1:
            break

    # si el usuario quiere ver los pasos, se dibujan
    if mostrar_graficos:
        graficar_pasos(V, aristas, pasos)

    return costo_total

def graficar_pasos(V, aristas, pasos, nombre_algoritmo="Kruskal"):
    """
    Dibuja paso a paso el proceso del MST.
    Incluye el nombre del algoritmo en el título y en el archivo.
    """
    # Crear el grafo base
    G = nx.Graph()
    G.add_nodes_from(V)
    for u, v, peso in aristas:
        G.add_edge(u, v, weight=peso)

    posiciones = nx.spring_layout(G, seed=10)
    etiquetas = nx.get_edge_attributes(G, 'weight')

    for i, (mst_actual, (u, v, w), accion) in enumerate(pasos, start=1):
        plt.figure(figsize=(6, 5))
        plt.title(f"{nombre_algoritmo} - Paso {i} ({accion})")

        # Grafo base
        nx.draw(G, posiciones, with_labels=True, node_color="lightblue", node_size=600)
        nx.draw_networkx_edge_labels(G, posiciones, edge_labels=etiquetas)

        # Aristas del MST actual (verde)
        if mst_actual:
            verdes = [(a, b) for (a, b, _) in mst_actual]
            nx.draw_networkx_edges(G, posiciones, edgelist=verdes, width=3, edge_color="green")

        # Arista actual
        if "agregado" in accion:
            nx.draw_networkx_edges(G, posiciones, edgelist=[(u, v)], width=5, edge_color="green")
        else:
            nx.draw_networkx_edges(G, posiciones, edgelist=[(u, v)], width=3, edge_color="red", style="dashed")

        plt.savefig(f"{nombre_algoritmo.lower()}_paso_{i}.png", dpi=150, bbox_inches="tight")
        plt.close()