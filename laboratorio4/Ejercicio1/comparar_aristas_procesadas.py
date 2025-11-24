"""Compara el número de aristas procesadas por Prim y Kruskal en un grafo de 6 vértices y 10 aristas."""
import random

# Importaciones robustas: soporta ejecución como paquete (python -m Ejercicio1...) o
# ejecución directa desde la carpeta `Ejercicio1` (python comparar_aristas_procesadas.py)
if __package__:
    from .kruskal.kruskal import kruskals_mst
    from .prim.prim import Grafo
else:
    from kruskal.kruskal import kruskals_mst
    from prim.prim import Grafo


def generar_grafo_conectado(num_vertices=6, num_aristas=10):
    if num_aristas < num_vertices - 1:
        raise ValueError("Se necesitan al menos n-1 aristas")
    vertices = list(range(num_vertices))
    aristas = []
    disponibles = vertices.copy()
    random.shuffle(disponibles)
    for i in range(1, num_vertices):
        u = disponibles[i]
        v = random.choice(disponibles[:i])
        peso = random.randint(1, 9)
        aristas.append((u, v, peso))
    while len(aristas) < num_aristas:
        u, v = random.sample(vertices, 2)
        peso = random.randint(1, 9)
        if all((a, b) != (u, v) and (b, a) != (u, v) for (a, b, _) in aristas):
            aristas.append((u, v, peso))
    return vertices, aristas


def main():
    V, aristas = generar_grafo_conectado(6, 10)
    print("Grafo: ", V)
    print("Aristas:")
    for a in aristas:
        print(a)

    aristas_k, costo_k, inspeccionadas = kruskals_mst(V, aristas, contar_inspeccionadas=True)
    g = Grafo(len(V))
    # construir matriz
    for u, v, w in aristas:
        g.matriz[u][v] = w
        g.matriz[v][u] = w
    # Elegir un vértice de inicio para Prim (según consigna) y mostrarlo
    vertice_inicio = random.choice(V)
    print(f"\nVértice inicial elegido para Prim: {vertice_inicio}")
    # Usar la versión con heap para Prim y contar pops (extracciones)
    aristas_p, costo_p, pops = g.prim_MST_heap(contar_pops=True, vertice_inicio=vertice_inicio)

    print(f"\nKruskal: aristas procesadas (inspeccionadas) = {inspeccionadas}, costo = {costo_k}")
    print(f"Prim (heap): extracciones del heap (pops) = {pops}, costo = {costo_p}")

    if costo_k == costo_p:
        print("Ambos algoritmos alcanzaron el mismo costo (MST).")
    else:
        print("Difieren en costo: revisar implementaciones.")


if __name__ == '__main__':
    main()
