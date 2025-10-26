def kruskal_mst(V, aristas):
    """Kruskal clásico: ordena aristas y usa Disjoint Set.

    Devuelve: (mst, costo, inspeccionadas)
    """
    # Ordenar por peso
    ar_sorted = sorted(aristas, key=lambda x: x[2])

    # Disjoint set simple
    n = len(V)
    parent = list(range(n))
    rank = [0] * n

    def find(x):
        if parent[x] != x:
            parent[x] = find(parent[x])
        return parent[x]

    def union(x, y):
        rx = find(x)
        ry = find(y)
        if rx == ry:
            return False
        if rank[rx] < rank[ry]:
            parent[rx] = ry
        elif rank[ry] < rank[rx]:
            parent[ry] = rx
        else:
            parent[ry] = rx
            rank[rx] += 1
        return True

    mst = []
    inspeccionadas = 0
    for u, v, w in ar_sorted:
        inspeccionadas += 1
        if find(u) != find(v):
            union(u, v)
            mst.append((u, v, w))
            if len(mst) == len(V) - 1:
                break

    costo = sum(w for (_, _, w) in mst)
    return mst, costo, inspeccionadas
