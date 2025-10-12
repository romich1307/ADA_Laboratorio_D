import math
import time
import random
"""Implementación de Disjoint Set (Union-Find) como clase Ciclos.

Este módulo reemplaza versiones antiguas que intentaban importar
funciones de otros módulos no presentes cuando se ejecuta el script
directamente. La clase Ciclos es independiente y ligera.
"""

class Ciclos:
    """Estructura Disjoint Set (Union-Find) con path compression y union by rank.

    Se usa en Kruskal para comprobar ciclos rápidamente.
    """

    def __init__(self, n):
        # parent[i] = padre de i; si parent[i] == i entonces es raíz
        self.parent = list(range(n))
        # rank (altura aproximada) para union by rank
        self.rank = [0] * n

    def encontrar(self, x):
        """Encuentra la raíz representante del conjunto que contiene x.

        Aplica compresión de camino para acelerar búsquedas posteriores.
        """
        if self.parent[x] != x:
            self.parent[x] = self.encontrar(self.parent[x])
        return self.parent[x]

    def union(self, x, y):
        """Une los conjuntos que contienen x e y. Devuelve True si se hizo unión.

        Si ya estaban unidos devuelve False.
        """
        rx = self.encontrar(x)
        ry = self.encontrar(y)
        if rx == ry:
            return False
        # unir por rank
        if self.rank[rx] < self.rank[ry]:
            self.parent[rx] = ry
        elif self.rank[ry] < self.rank[rx]:
            self.parent[ry] = rx
        else:
            self.parent[ry] = rx
            self.rank[rx] += 1
        return True


__all__ = ["Ciclos"]
# =====================================================
