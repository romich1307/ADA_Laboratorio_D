#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import heapq
from typing import List, Tuple, Dict

INF = 10**12

# (destino, tiempo, precio)
VueloInfo = Tuple[int, int, int]


# ---------- Funciones requeridas ----------

def agregar_vuelo(origen: int, destino: int, tiempo: int, precio: int,
                  grafo: List[List[VueloInfo]]) -> None:
    """
    Agrega una arista dirigida origen -> destino con peso 'tiempo' (minutos)
    y un 'precio' (dólares o la moneda que quieras).
    grafo es una lista de adyacencia: grafo[u] = [(v, tiempo, precio), ...]
    """
    if 0 <= origen < len(grafo) and 0 <= destino < len(grafo):
        grafo[origen].append((destino, tiempo, precio))


def dijkstra(origen: int, destino: int,
             grafo: List[List[VueloInfo]]) -> Tuple[List[int], int]:
    """
    Dijkstra con priority queue (heapq) usando TIEMPO como peso.
    Retorna (camino, tiempo_total).
    camino: lista de indices desde origen hasta destino (vacío si no hay ruta).
    tiempo_total: tiempo en minutos, -1 si no hay ruta.
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
        for v, w, _precio in grafo[u]:
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


# ---------- ORDENAMIENTO DIVIDE Y VENCERÁS (MERGESORT) ----------

def mergesort_vuelos_por_precio(vuelos: List[VueloInfo]) -> List[VueloInfo]:
    """
    Ordena una lista de vuelos por PRECIO usando MergeSort (Divide y Vencerás).

    - Divide: parte la lista en dos mitades.
    - Vence: ordena recursivamente cada mitad.
    - Combina: fusiona las dos mitades ordenadas.

    Complejidad: O(n log n)
    """
    if len(vuelos) <= 1:
        return vuelos

    mid = len(vuelos) // 2
    izquierda = mergesort_vuelos_por_precio(vuelos[:mid])
    derecha = mergesort_vuelos_por_precio(vuelos[mid:])
    return merge_por_precio(izquierda, derecha)


def merge_por_precio(a: List[VueloInfo], b: List[VueloInfo]) -> List[VueloInfo]:
    """Fusiona dos listas de vuelos ordenadas por precio."""
    i = j = 0
    resultado: List[VueloInfo] = []
    while i < len(a) and j < len(b):
        # Comparar por precio (índice 2 de la tupla)
        if a[i][2] <= b[j][2]:
            resultado.append(a[i])
            i += 1
        else:
            resultado.append(b[j])
            j += 1
    # Agregar lo que quede
    resultado.extend(a[i:])
    resultado.extend(b[j:])
    return resultado


def listar_vuelos_ordenados_por_precio(origen: int,
                                       grafo: List[List[VueloInfo]],
                                       nombres: List[str]) -> None:
    """
    Toma TODOS los vuelos que salen del 'origen' y los muestra
    ordenados por precio usando MergeSort (Divide y Vencerás).
    """
    vuelos_desde_origen = grafo[origen][:]
    if not vuelos_desde_origen:
        print("\nNo hay vuelos que salgan desde este país.")
        return

    vuelos_ordenados = mergesort_vuelos_por_precio(vuelos_desde_origen)

    print("\nVuelos directos que salen de", nombres[origen],
          "ordenados por PRECIO (Divide y Vencerás):")
    print("  Destino        Tiempo(min)   Precio")
    for destino, tiempo, precio in vuelos_ordenados:
        print(f"  {nombres[destino]:<13} {tiempo:>6}        {precio:>6}")


# ---------- Programa principal ----------

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
    grafo: List[List[VueloInfo]] = [[] for _ in range(n)]
    # mapa para busqueda por nombre (minusculas)
    mapa_nombres: Dict[str,int] = {nombre.lower(): i for i, nombre in enumerate(nombres)}

    # origen, destino, tiempo(min), precio (ejemplo)
    agregar_vuelo(0, 1, 180, 320, grafo)  # Argentina -> Chile
    agregar_vuelo(1, 2, 150, 280, grafo)  # Chile -> Peru
    agregar_vuelo(0, 3, 240, 450, grafo)  # Argentina -> Brasil
    agregar_vuelo(3, 2, 200, 400, grafo)  # Brasil -> Peru
    agregar_vuelo(0, 4, 120, 250, grafo)  # Argentina -> Bolivia
    agregar_vuelo(4, 2, 180, 260, grafo)  # Bolivia -> Peru
    agregar_vuelo(0, 6, 150, 230, grafo)  # Argentina -> Paraguay
    agregar_vuelo(6, 3, 100, 200, grafo)  # Paraguay -> Brasil
    agregar_vuelo(3, 2, 200, 350, grafo)  # Brasil -> Peru (otra aerolínea, otro precio)
    agregar_vuelo(0, 2, 420, 500, grafo)  # Argentina -> Peru directo
    agregar_vuelo(1, 4, 200, 260, grafo)  # Chile -> Bolivia
    agregar_vuelo(4, 3, 180, 300, grafo)  # Bolivia -> Brasil
    agregar_vuelo(5, 3, 120, 220, grafo)  # Uruguay -> Brasil
    agregar_vuelo(0, 5, 90, 210, grafo)   # Argentina -> Uruguay
    agregar_vuelo(7, 2, 170, 270, grafo)  # Ecuador -> Peru
    agregar_vuelo(1, 7, 200, 290, grafo)  # Chile -> Ecuador

    print("Planificador sencillo de vuelos")
    print(" - Dijkstra: ruta más rápida (minimiza TIEMPO)")
    print(" - MergeSort (Divide y Vencerás): ordena vuelos por PRECIO\n")
    print("Paises disponibles (indice : nombre):")
    for i, nombre in enumerate(nombres):
        print(f"  {i} : {nombre}")
    print("\nIntroduce INDICE (ej. 0) o NOMBRE exacto (ej. Argentina).")

    # Si quieres volver al input interactivo, descomenta estas dos líneas:
    # entrada_origen = input("Origen: ").strip()
    # entrada_destino = input("Destino: ").strip()

    # Para probar rápido sin escribir:
    entrada_origen = "Argentina"
    entrada_destino = "Peru"

    origen = leer_indice_pais(entrada_origen, mapa_nombres)
    destino = leer_indice_pais(entrada_destino, mapa_nombres)

    if origen < 0 or origen >= n or destino < 0 or destino >= n:
        print("Origen o destino no valido. Asegurate de escribir el indice o el nombre exacto.")
        return

    # Aquí usamos Divide y Vencerás (MergeSort) para ordenar por precio
    listar_vuelos_ordenados_por_precio(origen, grafo, nombres)

    # Aquí usamos Dijkstra para encontrar la ruta más rápida
    print(f"\nCalculando ruta mas rapida desde {nombres[origen]} hasta {nombres[destino]}...\n")
    camino, tiempo_total = dijkstra(origen, destino, grafo)

    if not camino:
        print(f"No existe una ruta posible entre {nombres[origen]} y {nombres[destino]}.")
    else:
        print("Ruta encontrada (criterio: TIEMPO mínimo):")
        imprimir_camino(camino, nombres)
        print(f"Tiempo total estimado: {tiempo_total} minutos")
        horas = tiempo_total // 60
        minutos = tiempo_total % 60
        print(f"  (aprox. {horas} h {minutos} min)")


if __name__ == "__main__":
    main()
