import sys


class Grafo:
    def __init__(self, vertices):
        self.n_vertices = vertices
        # matriz de adyacencia
        self.matriz = [[0 for _ in range(vertices)] for _ in range(vertices)]

    def imprimir_MST(self, padre):
        print("Arista \tPeso")
        for i in range(1, self.n_vertices):
            print(padre[i], "-", i, "\t", self.matriz[padre[i]][i])

    def obtener_MST(self, padre):
        """Devuelve una lista de aristas (u, v, peso) y el costo total del MST."""
        aristas = []
        costo = 0
        for i in range(1, self.n_vertices):
            u = padre[i]
            v = i
            peso = self.matriz[u][v]
            aristas.append((u, v, peso))
            costo += peso
        return aristas, costo

    def indice_minimo(self, claves, en_mst):
        minimo = sys.maxsize
        indice_min = -1
        for v in range(self.n_vertices):
            if claves[v] < minimo and en_mst[v] == False:
                minimo = claves[v]
                indice_min = v
        return indice_min

    def prim_MST(self):
        claves = [sys.maxsize] * self.n_vertices
        padre = [None] * self.n_vertices
        claves[0] = 0
        en_mst = [False] * self.n_vertices
        padre[0] = -1

        for _ in range(self.n_vertices):
            u = self.indice_minimo(claves, en_mst)
            if u == -1:
                break
            en_mst[u] = True
            for v in range(self.n_vertices):
                if self.matriz[u][v] > 0 and en_mst[v] == False and claves[v] > self.matriz[u][v]:
                    claves[v] = self.matriz[u][v]
                    padre[v] = u

        # al terminar construimos la lista de aristas y retornamos junto con el costo
        self.imprimir_MST(padre)
        return self.obtener_MST(padre)


def ejemplo():
    g = Grafo(5)
    g.matriz = [[0, 2, 0, 6, 0],
                [2, 0, 3, 8, 5],
                [0, 3, 0, 0, 7],
                [6, 8, 0, 0, 9],
                [0, 5, 7, 9, 0]]
    aristas, costo = g.prim_MST()
    print(f"Costo total Prim (ejemplo): {costo}")
    return aristas, costo


if __name__ == '__main__':
    ejemplo()
