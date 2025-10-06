from functools import cmp_to_key
from ciclos import Ciclos


def comparador(a, b):
    return a[2] - b[2]


def kruskals_mst(V, aristas):
    
    aristas = sorted(aristas, key=cmp_to_key(comparador))

    dsu = Ciclos(len(V))
    costo = 0
    contadot = 0
    for u, v, peso in aristas:
        contadot += 1
        if dsu.encontrar(u) != dsu.encontrar(v):
            dsu.union(u, v)
            costo += peso
            if contadot == len(V) - 1:
                break
    return costo
