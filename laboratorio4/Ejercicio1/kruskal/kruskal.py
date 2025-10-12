from functools import cmp_to_key

# Import robusto: permitir ejecución como paquete o como script independiente
try:
    from .ciclos import Ciclos
except Exception:
    try:
        from kruskal.ciclos import Ciclos
    except Exception:
        # último recurso: ruta absoluta por paquete del workspace
        from laboratorio4.Ejercicio1.kruskal.ciclos import Ciclos


def comparador(a, b):
    return a[2] - b[2]


def kruskals_mst(V, aristas, contar_inspeccionadas=False):
    """Devuelve (aristas_mst, costo) o (aristas_mst, costo, inspeccionadas).

    - V: iterable con vértices
    - aristas: lista de (u, v, peso)
    - contar_inspeccionadas: si True devuelve también el número de aristas inspeccionadas
    """
    aristas_ordenadas = sorted(aristas, key=cmp_to_key(comparador))

    dsu = Ciclos(len(V))
    costo = 0
    aristas_mst = []
    inspeccionadas = 0
    for u, v, peso in aristas_ordenadas:
        inspeccionadas += 1
        if dsu.encontrar(u) != dsu.encontrar(v):
            dsu.union(u, v)
            aristas_mst.append((u, v, peso))
            costo += peso
            if len(aristas_mst) == len(V) - 1:
                break

    if contar_inspeccionadas:
        return aristas_mst, costo, inspeccionadas
    return aristas_mst, costo
