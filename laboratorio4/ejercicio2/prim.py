import heapq
import networkx as nx
import matplotlib.pyplot as plt

def prim_mst(V, aristas, vertice_inicio=0, mostrar_graficos=False):
    """
    Algoritmo de Prim para generar el Árbol de Expansión Mínima (MST)
    - V: lista de vértices (ej. [0,1,2,3])
    - aristas: lista de tuplas (u, v, peso)
    - vertice_inicio: nodo desde donde comienza el MST
    - mostrar_graficos: si True, muestra los pasos con imágenes
    """

    # Crear lista de adyacencia a partir de la lista de aristas
    adyacencia = {v: [] for v in V}
    for u, v, peso in aristas:
        adyacencia[u].append((v, peso))
        adyacencia[v].append((u, peso))

    # Inicializar estructuras
    visitado = set()
    visitado.add(vertice_inicio)
    aristas_posibles = []
    mst = []
    pasos = []

    # Agregar aristas iniciales del vértice de inicio
    for vecino, peso in adyacencia[vertice_inicio]:
        heapq.heappush(aristas_posibles, (peso, vertice_inicio, vecino))

    # Mientras haya aristas por revisar
    while aristas_posibles and len(mst) < len(V) - 1:
        peso, origen, destino = heapq.heappop(aristas_posibles)

        if destino in visitado:
            pasos.append((mst.copy(), (origen, destino, peso), "descartado (ya visitado)"))
            continue

        # Agregar al MST
        mst.append((origen, destino, peso))
        pasos.append((mst.copy(), (origen, destino, peso), "agregado"))
        visitado.add(destino)

        # Añadir nuevas aristas del nuevo vértice
        for vecino, peso_vecino in adyacencia[destino]:
            if vecino not in visitado:
                heapq.heappush(aristas_posibles, (peso_vecino, destino, vecino))

    # Dibujar si se pide
    if mostrar_graficos:
        graficar_pasos(V, aristas, pasos)

    # Calcular el costo total
    costo_total = sum(peso for _, _, peso in mst)
    return costo_total


def graficar_pasos(V, aristas, pasos, nombre_algoritmo="Prim"):
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