#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import heapq
from typing import List, Tuple, Dict

INF = 10**12

# ---------- Funciones requeridas ----------

def agregar_vuelo(origen: int, destino: int, tiempo: int,
                  grafo: List[List[Tuple[int,int]]]) -> None:
    """
    Agrega una arista dirigida origen -> destino con peso 'tiempo' (minutos).
    grafo es una lista de adyacencia: grafo[u] = [(v, tiempo), ...]
    """
    if 0 <= origen < len(grafo) and 0 <= destino < len(grafo):
        grafo[origen].append((destino, tiempo))

def dijkstra(origen: int, destino: int,
             grafo: List[List[Tuple[int,int]]]) -> Tuple[List[int], int]:
    """
    Dijkstra con priority queue (heapq).
    Retorna (camino, tiempo_total).
    camino: lista de indices desde origen hasta destino (vacío si no hay ruta).
    tiempo_total: tiempo en minutos, -1 si no hay ruta.
    
    Complejidad: O((V + E) log V) (habitualmente O(E log V)), donde
    V = numero de nodos, E = numero de aristas.
    """
    n = len(grafo)
    dist = [INF] * n
    padre = [-1] * n
    dist[origen] = 0
    heap = [(0, origen)]  # (distancia acumulada, nodo)

    while heap:
        d_u, u = heapq.heappop(heap)
        if d_u > dist[u]:
            continue  # entrada obsoleta en la cola
        if u == destino:
            break  # podemos detenernos temprano
        for v, w in grafo[u]:
            if dist[u] + w < dist[v]:
                dist[v] = dist[u] + w
                padre[v] = u
                heapq.heappush(heap, (dist[v], v))

    if dist[destino] >= INF:
        return [], -1  # no hay ruta

    # Reconstruir camino
    camino = []
    v = destino
    while v != -1:
        camino.append(v)
        v = padre[v]
    camino.reverse()
    return camino, dist[destino]

def imprimir_camino(camino: List[int], nombres: List[str]) -> None:
    """Imprime el camino en nombres de paises"""
    if not camino:
        print("  (sin camino disponible)")
        return
    ruta = " -> ".join(nombres[i] for i in camino)
    print("  " + ruta)

def leer_indice_pais(entrada: str, mapa_nombres: Dict[str,int]) -> int:
    """
    Interpreta la entrada: si es un entero lo devuelve como indice,
    si es un texto busca el nombre (case-insensitive exacto).
    Retorna -1 si no se encuentra.
    """
    entrada = entrada.strip()
    # intentar interpretar como entero
    if entrada.isdigit():
        return int(entrada)
    clave = entrada.lower()
    return mapa_nombres.get(clave, -1)


def main():
    nombres = [
        "Argentina", # 0
        "Chile",     # 1
        "Peru",      # 2
        "Brasil",    # 3
        "Bolivia",   # 4
        "Uruguay",   # 5
        "Paraguay",  # 6
        "Ecuador"    # 7
    ]
    n = len(nombres)
    grafo: List[List[Tuple[int,int]]] = [[] for _ in range(n)]
    # mapa para busqueda por nombre (minusculas)
    mapa_nombres: Dict[str,int] = {nombre.lower(): i for i, nombre in enumerate(nombres)}

    agregar_vuelo(0, 1, 180, grafo)  # Argentina -> Chile (3h)
    agregar_vuelo(1, 2, 150, grafo)  # Chile -> Peru (2.5h)
    agregar_vuelo(0, 3, 240, grafo)  # Argentina -> Brasil (4h)
    agregar_vuelo(3, 2, 200, grafo)  # Brasil -> Peru (3h20)
    agregar_vuelo(0, 4, 120, grafo)  # Argentina -> Bolivia (2h)
    agregar_vuelo(4, 2, 180, grafo)  # Bolivia -> Peru (3h)
    agregar_vuelo(0, 6, 150, grafo)  # Argentina -> Paraguay (2.5h)
    agregar_vuelo(6, 3, 100, grafo)  # Paraguay -> Brasil (1.7h)
    agregar_vuelo(3, 2, 200, grafo)  # Brasil -> Peru (duplicado deliberado)
    agregar_vuelo(0, 2, 420, grafo)  # Argentina -> Peru directo (7h)
    agregar_vuelo(1, 4, 200, grafo)  # Chile -> Bolivia
    agregar_vuelo(4, 3, 180, grafo)  # Bolivia -> Brasil
    agregar_vuelo(5, 3, 120, grafo)  # Uruguay -> Brasil
    agregar_vuelo(0, 5, 90, grafo)   # Argentina -> Uruguay
    agregar_vuelo(7, 2, 170, grafo)  # Ecuador -> Peru
    agregar_vuelo(1, 7, 200, grafo)  # Chile -> Ecuador


    print("Planificador sencillo de vuelos (Dijkstra)")
    print("Paises disponibles (indice : nombre):")
    for i, nombre in enumerate(nombres):
        print(f"  {i} : {nombre}")
    print("\nIntroduce INDICE (ej. 0) o NOMBRE exacto (ej. Argentina).")

    entrada_origen = input("Origen: ").strip()
    entrada_destino = input("Destino: ").strip()

    origen = leer_indice_pais(entrada_origen, mapa_nombres)
    destino = leer_indice_pais(entrada_destino, mapa_nombres)

    if origen < 0 or origen >= n or destino < 0 or destino >= n:
        print("Origen o destino no valido. Asegurate de escribir el indice o el nombre exacto.")
        return

    print(f"\nCalculando ruta mas rapida desde {nombres[origen]} hasta {nombres[destino]}...\n")
    camino, tiempo_total = dijkstra(origen, destino, grafo)

    if not camino:
        print(f"No existe una ruta posible entre {nombres[origen]} y {nombres[destino]}.")
    else:
        print("Ruta encontrada:")
        imprimir_camino(camino, nombres)
        print(f"Tiempo total estimado: {tiempo_total} minutos")
        horas = tiempo_total // 60
        minutos = tiempo_total % 60
        print(f"  (aprox. {horas} h {minutos} min)")

if __name__ == "__main__":
    main()

