import os
import networkx as nx
import matplotlib.pyplot as plt


def leer_red_desde_txt(path):
    """Lee un archivo con formato simple:
    - primera línea: n (número de nodos)
    - siguientes líneas: u v peso (nodos numerados 0..n-1)
    Devuelve (V, aristas)
    """
    with open(path, 'r', encoding='utf-8') as f:
        lines = [l.strip() for l in f if l.strip() and not l.startswith('#')]
    n = int(lines[0])
    V = list(range(n))
    aristas = []
    for ln in lines[1:]:
        parts = ln.split()
        if len(parts) < 3:
            continue
        u = int(parts[0])
        v = int(parts[1])
        w = float(parts[2])
        aristas.append((u, v, w))
    return V, aristas


def dibujar_red(V, aristas, mst=None, filename=None, title=None):
    G = nx.Graph()
    G.add_nodes_from(V)
    for u, v, w in aristas:
        G.add_edge(u, v, weight=w)
    pos = nx.spring_layout(G, seed=42)
    plt.figure(figsize=(8, 6))
    if title:
        plt.title(title)
    nx.draw(G, pos, with_labels=True, node_color='lightblue')
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels)
    if mst:
        msa = [(a, b) for (a, b, _) in mst]
        nx.draw_networkx_edges(G, pos, edgelist=msa, width=3, edge_color='green')
    if filename:
        os.makedirs(os.path.dirname(filename), exist_ok=True)
        plt.savefig(filename, dpi=150, bbox_inches='tight')
    plt.close()
