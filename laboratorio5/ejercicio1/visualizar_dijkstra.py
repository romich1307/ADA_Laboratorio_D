import networkx as nx
import matplotlib.pyplot as plt

# Grafo con 8 vértices y 15 aristas
def visualizar_dijkstra():
    # Crear el grafo
    G = nx.Graph()
    
    # Añadir vértices (0 a 7)
    G.add_nodes_from(range(8))
    
    # Definir las aristas (las mismas que en el código C++)
    # Estructura: (u, v, peso)
    aristas = [
        (0, 1, 4),   # Vértice 0 -> 1
        (0, 2, 3),   # Vértice 0 -> 2
        (1, 2, 2),   # Vértice 1 -> 2
        (1, 3, 7),   # Vértice 1 -> 3
        (2, 3, 5),   # Vértice 2 -> 3
        (2, 4, 8),   # Vértice 2 -> 4
        (3, 4, 1),   # Vértice 3 -> 4
        (3, 5, 9),   # Vértice 3 -> 5
        (4, 5, 6),   # Vértice 4 -> 5
        (4, 6, 10),  # Vértice 4 -> 6
        (5, 6, 2),   # Vértice 5 -> 6
        (5, 7, 4),   # Vértice 5 -> 7
        (6, 7, 3),   # Vértice 6 -> 7
        (0, 7, 9),   # Vértice 0 -> 7
        (1, 4, 5)    # Vértice 1 -> 4
    ]
    
    # Añadir las aristas con pesos
    G.add_weighted_edges_from(aristas)
    
    # Solicitar al usuario el vértice origen
    print("=== VISUALIZACIÓN DEL ALGORITMO DE DIJKSTRA ===")
    origen = int(input(f"Ingrese el vértice origen (0-7): "))
    
    if origen < 0 or origen > 7:
        print("Error: Vértice inválido")
        return
    
    # Calcular las distancias más cortas usando Dijkstra de NetworkX
    distancias = nx.single_source_dijkstra_path_length(G, origen)
    caminos = nx.single_source_dijkstra_path(G, origen)
    
    # Mostrar resultados en consola
    print(f"\nDistancias más cortas desde el vértice {origen}:")
    print("-----------------------------------")
    print("Vértice\t\tDistancia\tCamino")
    print("-----------------------------------")
    for vertice in range(8):
        if vertice in distancias:
            camino = " -> ".join(map(str, caminos[vertice]))
            print(f"   {vertice}\t\t{distancias[vertice]}\t\t{camino}")
        else:
            print(f"   {vertice}\t\tINF\t\tIinalcanzable")
    print("-----------------------------------")
    
    # Crear figura con dos subgráficos
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 7))
    
    # Configurar layout del grafo
    pos = nx.spring_layout(G, seed=42, k=2)
    
    # --- GRÁFICO 1: Grafo original ---
    ax1.set_title("Grafo Original (8 vértices, 15 aristas)", fontsize=14, fontweight='bold')
    
    # Dibujar nodos
    nx.draw_networkx_nodes(G, pos, node_color='lightblue', 
                          node_size=800, ax=ax1)
    
    # Dibujar aristas
    nx.draw_networkx_edges(G, pos, width=2, alpha=0.6, ax=ax1)
    
    # Dibujar etiquetas de nodos
    nx.draw_networkx_labels(G, pos, font_size=12, 
                           font_weight='bold', ax=ax1)
    
    # Dibujar pesos de las aristas
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels, font_size=10, ax=ax1)
    
    ax1.axis('off')
    
    # --- GRÁFICO 2: Caminos más cortos desde el origen ---
    ax2.set_title(f"Caminos más cortos desde vértice {origen} (Dijkstra)", 
                 fontsize=14, fontweight='bold')
    
    # Dibujar todos los nodos
    colores_nodos = []
    for nodo in range(8):
        if nodo == origen:
            colores_nodos.append('green')  # Nodo origen en verde
        elif nodo in distancias:
            colores_nodos.append('lightcoral')  # Nodos alcanzables
        else:
            colores_nodos.append('gray')  # Nodos inalcanzables
    
    nx.draw_networkx_nodes(G, pos, node_color=colores_nodos, 
                          node_size=800, ax=ax2)
    
    # Resaltar las aristas que forman parte de los caminos más cortos
    aristas_camino = set()
    for destino, camino in caminos.items():
        for i in range(len(camino) - 1):
            u, v = camino[i], camino[i + 1]
            aristas_camino.add((min(u, v), max(u, v)))
    
    # Dibujar aristas normales
    aristas_normales = [e for e in G.edges() if (min(e), max(e)) not in aristas_camino]
    nx.draw_networkx_edges(G, pos, edgelist=aristas_normales, 
                          width=1, alpha=0.2, style='dashed', ax=ax2)
    
    # Dibujar aristas de los caminos más cortos
    aristas_camino_list = [e for e in G.edges() if (min(e), max(e)) in aristas_camino]
    nx.draw_networkx_edges(G, pos, edgelist=aristas_camino_list, 
                          width=3, alpha=0.8, edge_color='red', ax=ax2)
    
    # Dibujar etiquetas de nodos
    nx.draw_networkx_labels(G, pos, font_size=12, 
                           font_weight='bold', ax=ax2)
    
    # Dibujar pesos de las aristas
    nx.draw_networkx_edge_labels(G, pos, edge_labels, font_size=10, ax=ax2)
    
    # Añadir leyenda de distancias
    leyenda_texto = f"Distancias desde vértice {origen}:\n"
    for vertice in range(8):
        if vertice in distancias:
            leyenda_texto += f"  {vertice}: {distancias[vertice]}\n"
        else:
            leyenda_texto += f"  {vertice}: INF\n"
    
    ax2.text(0.02, 0.98, leyenda_texto, transform=ax2.transAxes,
            fontsize=10, verticalalignment='top',
            bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))
    
    ax2.axis('off')
    
    plt.tight_layout()
    plt.savefig('dijkstra_visualizacion.png', dpi=300, bbox_inches='tight')
    print("\n✓ Gráfico guardado como 'dijkstra_visualizacion.png'")
    plt.show()

if __name__ == "__main__":
    visualizar_dijkstra()
