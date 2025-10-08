import math
import time

def prim_matriz(matriz):
    """
    Versión del algoritmo de Prim que usa una matriz de adyacencia.
    matriz[i][j] = peso de la arista (i,j) o math.inf si no existe.
    """
    n = len(matriz)
    seleccionado = [False] * n
    costo_total = 0
    aristas_mst = []

    # Comenzar desde el vértice 0
    seleccionado[0] = True

    while len(aristas_mst) < n - 1:
        minimo = math.inf
        u = v = -1

        for i in range(n):
            if seleccionado[i]:
                for j in range(n):
                    if not seleccionado[j] and matriz[i][j] != math.inf:
                        if matriz[i][j] < minimo:
                            minimo = matriz[i][j]
                            u, v = i, j

        if u != -1 and v != -1:
            seleccionado[v] = True
            aristas_mst.append((u, v, minimo))
            costo_total += minimo
        else:
            break  # el grafo no está conectado

    return costo_total, aristas_mst
