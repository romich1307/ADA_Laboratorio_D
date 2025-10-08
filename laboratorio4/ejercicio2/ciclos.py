

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
