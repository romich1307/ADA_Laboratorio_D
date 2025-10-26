import random
import os
import networkx as nx
import matplotlib.pyplot as plt


def generar_grafo_conectado(n, m, seed=None, peso_max=100):
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
        w = random.randint(1, peso_max)
        aristas.append((u, v, w))
    # añadir aristas adicionales
    while len(aristas) < m:
        u, v = random.sample(V, 2)
        if any(((a == u and b == v) or (a == v and b == u)) for (a, b, _) in aristas):
            continue
        w = random.randint(1, peso_max)
        aristas.append((u, v, w))
    return V, aristas


def edges_set_normalized(mst):
    return set(((min(a, b), max(a, b), w) for (a, b, w) in mst))


def dibujar_mst(V, aristas, mst, filename, title=None):
    """Dibuja el grafo completo y resalta el MST.

    Mejores prácticas para grafos grandes:
    - No mostrar etiquetas de arista cuando n>30
    - Usar alpha en aristas base y color verde en MST
    - Ajustar node_size según n
    - Usar seed fijo para layout reproducible
    """
    G = nx.Graph()
    G.add_nodes_from(V)
    for u, v, w in aristas:
        G.add_edge(u, v, weight=w)

    n = len(V)
    # configuraciones dependiendo del tamaño
    if n <= 30:
        figsize = (8, 6)
        node_size = 600
        font_size = 10
        show_edge_labels = True
        edge_alpha = 0.5
    elif n <= 70:
        figsize = (10, 8)
        node_size = 300
        font_size = 8
        show_edge_labels = False
        edge_alpha = 0.4
    else:
        figsize = (12, 10)
        node_size = 160
        font_size = 6
        show_edge_labels = False
        edge_alpha = 0.25

    pos = nx.spring_layout(G, seed=42, k=0.15)
    edge_labels = nx.get_edge_attributes(G, 'weight')

    plt.figure(figsize=figsize)
    if title:
        plt.title(title)

    # dibujar grafo base
    nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=node_size, font_size=font_size)
    # dibujar todas las aristas en gris tenue
    nx.draw_networkx_edges(G, pos, alpha=edge_alpha)

    # mostrar etiquetas de arista solo en grafos pequeños
    if show_edge_labels:
        nx.draw_networkx_edge_labels(G, pos, edge_labels)

    # destacar MST
    mst_edges = [(a, b) for (a, b, _) in mst]
    nx.draw_networkx_edges(G, pos, edgelist=mst_edges, width=3, edge_color='green')

    # guardar
    os.makedirs(os.path.dirname(filename), exist_ok=True)
    plt.savefig(filename, dpi=150, bbox_inches='tight')
    plt.close()
