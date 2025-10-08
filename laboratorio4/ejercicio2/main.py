from kruskal import kruskals_mst
from prim import prim_mst
import random

def generar_grafo_conectado(num_vertices=6, num_aristas=10):
    """
    Genera un grafo no dirigido conectado con num_vertices y num_aristas.
    Asegura que todos los vértices estén unidos al menos una vez.
    """
    if num_aristas < num_vertices - 1:
        raise ValueError("Se necesitan al menos n−1 aristas para mantener la conexión.")

    vertices = list(range(num_vertices))
    aristas = []

    # 1️⃣ Crear un árbol base para asegurar conectividad
    disponibles = vertices.copy()
    random.shuffle(disponibles)
    for i in range(1, num_vertices):
        u = disponibles[i]
        v = random.choice(disponibles[:i])
        peso = random.randint(1, 9)
        aristas.append((u, v, peso))

    # 2️⃣ Agregar más aristas aleatorias hasta llegar a 10
    while len(aristas) < num_aristas:
        u, v = random.sample(vertices, 2)
        peso = random.randint(1, 9)
        if all((a, b) != (u, v) and (b, a) != (u, v) for (a, b, _) in aristas):
            aristas.append((u, v, peso))

    return vertices, aristas


def ejemplo():
    # Generar grafo aleatorio conectado con 6 vértices y 10 aristas
    vertices, aristas = generar_grafo_conectado(6, 10)

    print("=== GRAFO GENERADO ===")
    print(f"Vértices: {vertices}")
    print("Aristas (u, v, peso):")
    for a in aristas:
        print(a)

    print("\n=== ALGORITMO DE KRUSKAL ===")
    costo_kruskal = kruskals_mst(vertices, aristas, mostrar_graficos=True)
    print(f"Costo total del MST (Kruskal): {costo_kruskal}\n")

    print("=== ALGORITMO DE PRIM ===")
    costo_prim = prim_mst(vertices, aristas, vertice_inicio=0, mostrar_graficos=True)
    print(f"Costo total del MST (Prim): {costo_prim}\n")

    # Comparación
    if costo_kruskal == costo_prim:
        print("✅ Ambos algoritmos generaron el mismo costo total del MST.")
    else:
        print("⚠️ Los algoritmos dieron resultados distintos (revisar aristas o grafo).")


if __name__ == "__main__":
    ejemplo()
