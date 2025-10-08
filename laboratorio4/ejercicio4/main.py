import math, time
from ciclos import generar_grafo_conectado
from prim_mst_lista import prim_mst_lista
from prim_matriz import prim_matriz
from kruskal_mst_lista import kruskal_mst_lista
from kruskal_matriz import kruskal_matriz

def convertir_a_matriz(vertices, aristas):
    n = len(vertices)
    matriz = [[math.inf]*n for _ in range(n)]
    for u, v, peso in aristas:
        matriz[u][v] = peso
        matriz[v][u] = peso
    return matriz

def comparar_algoritmos():
    vertices, aristas = generar_grafo_conectado(6, 10)
    matriz = convertir_a_matriz(vertices, aristas)

    print("=== Comparación de Prim y Kruskal ===")
    print("Vértices:", vertices)
    print("Aristas:", aristas, "\n")

    # ---- PRIM ----
    t1 = time.time()
    costo_prim_lista, _ = prim_mst_lista(vertices, aristas)
    t2 = time.time()

    t3 = time.time()
    costo_prim_matriz, _ = prim_matriz(matriz)
    t4 = time.time()

    print(f"Prim (lista adyacencia): costo={costo_prim_lista}, tiempo={(t2-t1)*1000:.3f} ms")
    print(f"Prim (matriz adyacencia): costo={costo_prim_matriz}, tiempo={(t4-t3)*1000:.3f} ms\n")

    # ---- KRUSKAL ----
    t5 = time.time()
    costo_kruskal_lista, _ = kruskal_mst_lista(vertices, aristas)
    t6 = time.time()

    t7 = time.time()
    costo_kruskal_matriz, _ = kruskal_matriz(matriz)
    t8 = time.time()

    print(f"Kruskal (lista adyacencia): costo={costo_kruskal_lista}, tiempo={(t6-t5)*1000:.3f} ms")
    print(f"Kruskal (matriz adyacencia): costo={costo_kruskal_matriz}, tiempo={(t8-t7)*1000:.3f} ms")

if __name__ == "__main__":
    comparar_algoritmos()
