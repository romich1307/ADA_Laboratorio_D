from ciclos import Ciclos
import math
import time

def kruskal_matriz(matriz):
    """
    Versión de Kruskal que recibe una matriz de adyacencia.
    """
    n = len(matriz)
    aristas = []

    # Convertir matriz en lista de aristas
    for i in range(n):
        for j in range(i+1, n):
            if matriz[i][j] != math.inf:
                aristas.append((i, j, matriz[i][j]))

    # Aplicar Kruskal normal
    aristas.sort(key=lambda x: x[2])
    dsu = Ciclos(n)
    mst = []
    costo_total = 0

    for u, v, peso in aristas:
        if dsu.encontrar(u) != dsu.encontrar(v):
            dsu.union(u, v)
            mst.append((u, v, peso))
            costo_total += peso

    return costo_total, mst
