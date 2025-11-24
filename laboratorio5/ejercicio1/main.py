## Implementar el algoritmo de Dijkstra para un grafo con 8 vértices y 15 aristas. Los pesos 
# son aleatorios entre 1 y 10; y encontrar el camino más corto desde un vértice dado a todos 
# los demás.

import heapq
import random
from collections import defaultdict

def construct_adj(edges, V):
    """
    Construye una lista de adyacencia a partir de las aristas.
    adj[u] = lista de tuplas (v, peso)
    """
    adj = defaultdict(list) 
    for u, v, wt in edges:
        adj[u].append((v, wt))
        adj[v].append((u, wt))  # Grafo no dirigido
    return adj

def dijkstra(V, edges, src):
    """
    Retorna las distancias más cortas desde src a todos los demás vértices.
    """
    # Crear lista de adyacencia
    adj = construct_adj(edges, V)
    
    # Cola de prioridad (min-heap) para almacenar (distancia, vértice)
    pq = [(0, src)]
    
    # Vector de distancias inicializado con infinito
    dist = [float('inf')] * V
    dist[src] = 0
    
    # Conjunto para mantener vértices visitados
    visited = set()
    
    while pq:
        # Extraer el vértice con la menor distancia
        current_dist, u = heapq.heappop(pq)
        
        # Si ya fue visitado, continuar
        if u in visited:
            continue
        
        visited.add(u)
        
        # Revisar todos los vecinos de u
        for v, weight in adj[u]:
            # Si hay un camino más corto a v a través de u
            if dist[u] + weight < dist[v]:
                dist[v] = dist[u] + weight
                heapq.heappush(pq, (dist[v], v))
    
    return dist

def generate_random_edges(V, num_edges):
    """
    Genera aristas aleatorias para un grafo con V vértices.
    Asegura que el grafo sea conexo.
    """
    edges = []
    used_edges = set()
    
    # Primero, crear un árbol de expansión para garantizar conectividad
    for i in range(1, V):
        u = random.randint(0, i - 1)
        v = i
        weight = random.randint(1, 10)
        edges.append((u, v, weight))
        used_edges.add((min(u, v), max(u, v)))
    
    # Agregar aristas adicionales aleatoriamente
    while len(edges) < num_edges:
        u = random.randint(0, V - 1)
        v = random.randint(0, V - 1)
        
        if u != v:
            edge = (min(u, v), max(u, v))
            if edge not in used_edges:
                weight = random.randint(1, 10)
                edges.append((u, v, weight))
                used_edges.add(edge)
    
    return edges

def main():
    V = 8  # 8 vértices
    num_edges = 15  # 15 aristas
    src = 0  # Vértice de origen
    
    # Generar aristas aleatorias
    edges = generate_random_edges(V, num_edges)
    
    print(f"Grafo con {V} vértices y {num_edges} aristas:")
    print("\nAristas (formato: u -> v con peso):")
    for u, v, weight in edges:
        print(f"  {u} -> {v} con peso {weight}")
    
    # Ejecutar algoritmo de Dijkstra
    result = dijkstra(V, edges, src)
    
    print(f"\nDistancias más cortas desde el vértice {src}:")
    for i, dist in enumerate(result):
        if dist == float('inf'):
            print(f"  Vértice {i}: ∞ (no alcanzable)")
        else:
            print(f"  Vértice {i}: {dist}")

if __name__ == "__main__":
    main()
