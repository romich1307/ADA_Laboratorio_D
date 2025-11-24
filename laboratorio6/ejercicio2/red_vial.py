from typing import List, Optional


class RedVialArequipa:
    """
    Gestiona la red vial de Arequipa y calcula distancias mínimas
    usando el algoritmo Floyd-Warshall
    """

    def __init__(self, num_nodos: int):
        self.n = num_nodos
        self.INF = float('inf')
        self.dist = [[self.INF] * num_nodos for _ in range(num_nodos)]
        self.next = [[None] * num_nodos for _ in range(num_nodos)]
        for i in range(num_nodos):
            self.dist[i][i] = 0
            self.next[i][i] = i
        self.nombres = {}

    def agregar_nodo(self, id_nodo: int, nombre: str):
        self.nombres[id_nodo] = nombre

    def agregar_calle(self, origen: int, destino: int, distancia_km: float):
        self.dist[origen][destino] = distancia_km
        self.dist[destino][origen] = distancia_km
        self.next[origen][destino] = destino
        self.next[destino][origen] = origen

    def calcular_floyd_warshall(self):
        print("\n" + "="*70)
        print("EJECUTANDO FLOYD-WARSHALL")
        print("="*70)
        print(f"Calculando distancias mínimas entre {self.n} nodos...")
        print(f"Complejidad: O({self.n}³) = {self.n**3:,} operaciones\n")
        operaciones = 0
        for k in range(self.n):
            print(f"Iteración {k+1}/{self.n}: Considerando nodo intermedio '{self.nombres.get(k, k)}'")
            for i in range(self.n):
                for j in range(self.n):
                    operaciones += 1
                    if self.dist[i][k] + self.dist[k][j] < self.dist[i][j]:
                        self.dist[i][j] = self.dist[i][k] + self.dist[k][j]
                        self.next[i][j] = self.next[i][k]
        print(f"\n✓ Floyd-Warshall completado: {operaciones:,} operaciones realizadas")
        print("✓ Matriz de distancias mínimas calculada\n")

    def obtener_distancia(self, origen: int, destino: int) -> float:
        return self.dist[origen][destino]

    def obtener_ruta(self, origen: int, destino: int) -> List[int]:
        if self.next[origen][destino] is None:
            return []
        ruta = [origen]
        actual = origen
        while actual != destino:
            actual = self.next[actual][destino]
            ruta.append(actual)
        return ruta

    def mostrar_matriz_distancias(self):
        print("\n" + "="*70)
        print("MATRIZ DE DISTANCIAS MÍNIMAS (km)")
        print("="*70)
        print(f"{'Desde/Hacia':<20}", end="")
        for j in range(self.n):
            print(f"{self.nombres.get(j, j):<12}", end="")
        print()
        print("-" * 70)
        for i in range(self.n):
            print(f"{self.nombres.get(i, i):<20}", end="")
            for j in range(self.n):
                dist = self.dist[i][j]
                if dist == self.INF:
                    print(f"{'∞':<12}", end="")
                else:
                    print(f"{dist:<12.2f}", end="")
            print()
        print()
