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

    def prim_MST(self, contar_extraidas=False, vertice_inicio=0):
        claves = [sys.maxsize] * self.n_vertices
        padre = [None] * self.n_vertices
        claves[vertice_inicio] = 0
        en_mst = [False] * self.n_vertices
        padre[vertice_inicio] = -1

        extraidas = 0
        for _ in range(self.n_vertices):
            u = self.indice_minimo(claves, en_mst)
            if u == -1:
                break
            en_mst[u] = True
            # Contamos cuántas aristas (entradas de frontera) consideramos al actualizar
            for v in range(self.n_vertices):
                if self.matriz[u][v] > 0 and en_mst[v] == False and claves[v] > self.matriz[u][v]:
                    claves[v] = self.matriz[u][v]
                    padre[v] = u
                if self.matriz[u][v] > 0:
                    extraidas += 1

        # al terminar construimos la lista de aristas y retornamos junto con el costo
        self.imprimir_MST(padre)
        aristas_mst, costo = self.obtener_MST(padre)
        if contar_extraidas:
            return aristas_mst, costo, extraidas
        return aristas_mst, costo

    def prim_MST_heap(self, contar_pops=False, vertice_inicio=0):
        """Versión de Prim usando un min-heap (heapq).

        Devuelve (aristas, costo) o (aristas, costo, pops) si contar_pops=True.
        pops cuenta cuántas extracciones (heap pops) se realizaron.
        """
        import heapq

        n = self.n_vertices
        visitado = [False] * n
        heap = [(0, vertice_inicio, -1)]  # (peso, vertice, padre)
        mst = []
        costo = 0
        pops = 0

        # padres usados solo para impresión si se desea
        padres = [None] * n

        while heap and len(mst) < n - 1:
            peso, u, padre = heapq.heappop(heap)
            pops += 1
            if visitado[u]:
                continue
            visitado[u] = True
            padres[u] = padre
            if padre != -1:
                mst.append((padre, u, peso))
                costo += peso
            # push de todos los vecinos no visitados
            for v in range(n):
                w = self.matriz[u][v]
                if w > 0 and not visitado[v]:
                    heapq.heappush(heap, (w, v, u))

        # imprimir resultado similar a imprimir_MST
        print("Arista \tPeso")
        for u, v, w in mst:
            print(f"{u} - {v}    {w}")

        if contar_pops:
            return mst, costo, pops
        return mst, costo


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
