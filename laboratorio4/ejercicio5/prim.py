import heapq

def prim_heap_mst(V, aristas, inicio=0):
    ady = {v: [] for v in V}
    for u, v, w in aristas:
        ady[u].append((v, w))
        ady[v].append((u, w))

    visitado = set()
    visitado.add(inicio)
    heap = []
    for vecino, peso in ady[inicio]:
        heapq.heappush(heap, (peso, inicio, vecino))

    mst = []
    pops = 0
    while heap and len(mst) < len(V) - 1:
        peso, u, v = heapq.heappop(heap)
        pops += 1
        if v in visitado:
            continue
        mst.append((u, v, peso))
        visitado.add(v)
        for vecino, w in ady[v]:
            if vecino not in visitado:
                heapq.heappush(heap, (w, v, vecino))

    costo = sum(w for (_, _, w) in mst)
    return mst, costo, pops
