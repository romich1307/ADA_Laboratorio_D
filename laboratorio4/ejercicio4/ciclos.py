
import random


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


class Ciclos:
    def __init__(self, cantidad):
        # cada nodo empieza siendo su propio padre
        self.padre = list(range(cantidad))
        # rango (altura aproximada) para union by rank
        self.rango = [1] * cantidad

    def encontrar(self, i):
        if self.padre[i] != i:
            self.padre[i] = self.encontrar(self.padre[i])
        return self.padre[i]

    def union(self, nodo_u, nodo_v):
        raiz_u = self.encontrar(nodo_u)
        raiz_v = self.encontrar(nodo_v)
        if raiz_u != raiz_v:
            if self.rango[raiz_u] > self.rango[raiz_v]:
                self.padre[raiz_v] = raiz_u
            elif self.rango[raiz_u] < self.rango[raiz_v]:
                self.padre[raiz_u] = raiz_v
            else:
                self.padre[raiz_v] = raiz_u
                self.rango[raiz_u] += 1
