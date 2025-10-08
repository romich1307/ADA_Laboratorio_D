import heapq

def prim_mst_lista(V, aristas, vertice_inicio=0):
    """
    Algoritmo de Prim usando lista de adyacencia.
    Retorna el costo total del MST y la lista de aristas seleccionadas.
    """
    # Crear lista de adyacencia
    adyacencia = {v: [] for v in V}
    for u, v, peso in aristas:
        adyacencia[u].append((v, peso))
        adyacencia[v].append((u, peso))

    visitado = set([vertice_inicio])
    aristas_posibles = []
    mst = []

    # Agregar aristas del nodo inicial
    for vecino, peso in adyacencia[vertice_inicio]:
        heapq.heappush(aristas_posibles, (peso, vertice_inicio, vecino))

    # Bucle principal
    while aristas_posibles and len(mst) < len(V) - 1:
        peso, origen, destino = heapq.heappop(aristas_posibles)
        if destino in visitado:
            continue
        visitado.add(destino)
        mst.append((origen, destino, peso))
        for vecino, peso_vecino in adyacencia[destino]:
            if vecino not in visitado:
                heapq.heappush(aristas_posibles, (peso_vecino, destino, vecino))

    costo_total = sum(p for _, _, p in mst)
    return costo_total, mst
