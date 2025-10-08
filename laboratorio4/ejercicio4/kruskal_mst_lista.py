from functools import cmp_to_key
from ciclos import Ciclos

def comparador(a, b):
    return a[2] - b[2]

def kruskal_mst_lista(V, aristas):
    """
    Implementación de Kruskal usando lista de aristas (estructura normal).
    Retorna el costo total y la lista de aristas seleccionadas.
    """
    # Ordenar las aristas por peso
    aristas = sorted(aristas, key=cmp_to_key(comparador))

    dsu = Ciclos(len(V))
    mst = []
    costo_total = 0

    for u, v, peso in aristas:
        # Solo unir si no forman ciclo
        if dsu.encontrar(u) != dsu.encontrar(v):
            dsu.union(u, v)
            mst.append((u, v, peso))
            costo_total += peso

        # Si ya tenemos n−1 aristas, terminamos
        if len(mst) == len(V) - 1:
            break

    return costo_total, mst
