import math
import time
import random
from prim_mst_lista import prim_mst_lista
from prim_matriz import prim_mst_matriz
from kruskal_mst_lista import kruskal_mst_lista
from kruskal_matriz import kruskal_mst_matriz



# =====================================================
# FUNCIÓN PARA GENERAR UN GRAFO CONECTADO
# =====================================================
def generar_grafo_conectado(num_vertices=6, num_aristas=10):
    """Genera un grafo no dirigido conectado con num_vertices y num_aristas."""
    if num_aristas < num_vertices - 1:
        raise ValueError("Se necesitan al menos n−1 aristas para mantener la conexión.")

    vertices = list(range(num_vertices))
    aristas = []

    # Crear un árbol base conectado
    disponibles = vertices.copy()
    random.shuffle(disponibles)
    for i in range(1, num_vertices):
        u = disponibles[i]
        v = random.choice(disponibles[:i])
        peso = random.randint(1, 9)
        aristas.append((u, v, peso))

    # Agregar aristas adicionales hasta completar el total
    while len(aristas) < num_aristas:
        u, v = random.sample(vertices, 2)
        peso = random.randint(1, 9)
        if all((a, b) != (u, v) and (b, a) != (u, v) for (a, b, _) in aristas):
            aristas.append((u, v, peso))

    return vertices, aristas


# =====================================================
# CONVERTIR A MATRIZ DE ADYACENCIA
# =====================================================
def convertir_a_matriz(vertices, aristas):
    n = len(vertices)
    matriz = [[math.inf] * n for _ in range(n)]
    for u, v, peso in aristas:
        matriz[u][v] = peso
        matriz[v][u] = peso
    return matriz


# =====================================================
# COMPARACIÓN ENTRE PRIM Y KRUSKAL
# =====================================================
def comparar_algoritmos():
    vertices, aristas = generar_grafo_conectado(6, 10)
    matriz = convertir_a_matriz(vertices, aristas)

    print("=== COMPARACIÓN DE PRIM Y KRUSKAL ===")
    print("Vértices:", vertices)
    print("Aristas:", aristas, "\n")

    # ---- PRIM ----
    t1 = time.time()
    costo_prim_lista, _ = prim_mst_lista(vertices, aristas)
    t2 = time.time()

    t3 = time.time()
    costo_prim_matriz, _ = prim_mst_matriz(matriz)
    t4 = time.time()

    print(f"Prim (lista adyacencia):   costo={costo_prim_lista}, tiempo={(t2-t1)*1000:.3f} ms")
    print(f"Prim (matriz adyacencia):  costo={costo_prim_matriz}, tiempo={(t4-t3)*1000:.3f} ms\n")

    # ---- KRUSKAL ----
    t5 = time.time()
    costo_kruskal_lista, _ = kruskal_mst_lista(vertices, aristas)
    t6 = time.time()

    t7 = time.time()
    costo_kruskal_matriz, _ = kruskal_mst_matriz(matriz)
    t8 = time.time()

    print(f"Kruskal (lista adyacencia): costo={costo_kruskal_lista}, tiempo={(t6-t5)*1000:.3f} ms")
    print(f"Kruskal (matriz adyacencia): costo={costo_kruskal_matriz}, tiempo={(t8-t7)*1000:.3f} ms")


# =====================================================
# PROGRAMA PRINCIPAL
# =====================================================
if __name__ == "__main__":
    comparar_algoritmos()
