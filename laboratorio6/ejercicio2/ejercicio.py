from dataclasses import dataclass
from typing import List, Tuple
import math
import heapq

# ==============================
#  MODELO: BARRIO / COMUNIDAD
# ==============================

@dataclass
class Barrio:
    id: int              # Índice del nodo en la red vial
    nombre: str
    demanda_litros: int  # Litros que requiere el barrio
    personas: int        # Habitantes beneficiados


# =====================================
#  GRAFO VIAL + DIJKSTRA DESDE PLANTA
# =====================================

class RedVialCisterna:
    """
    Grafo simple con lista de adyacencia y Dijkstra
    para calcular las distancias mínimas desde la planta.
    """

    def __init__(self, nombres_nodos: List[str],
                 aristas: List[Tuple[int, int, float]],
                 planta_idx: int = 0) -> None:
        self.nombres_nodos = nombres_nodos
        self.n = len(nombres_nodos)
        self.planta = planta_idx

        # adj[u] = lista de (v, distancia_km)
        self.adj: List[List[Tuple[int, float]]] = [[] for _ in range(self.n)]
        for u, v, d in aristas:
            self.adj[u].append((v, d))
            self.adj[v].append((u, d))  # grafo no dirigido

    def dijkstra_desde_planta(self) -> List[float]:
        """
        Implementación clásica de Dijkstra con cola de prioridad.
        Complejidad: O((V + E) log V).
        """
        dist = [math.inf] * self.n
        dist[self.planta] = 0.0

        pq: List[Tuple[float, int]] = [(0.0, self.planta)]

        while pq:
            d, u = heapq.heappop(pq)
            if d != dist[u]:
                continue

            for v, w in self.adj[u]:
                nd = d + w
                if nd < dist[v]:
                    dist[v] = nd
                    heapq.heappush(pq, (nd, v))

        return dist


# =========================================================
#  OPTIMIZADOR: DIJKSTRA + MOCHILA 0/1 + RUTA HEURÍSTICA
# =========================================================

class OptimizadorCisterna:
    """
    Combina Dijkstra + Mochila 0/1 para elegir qué barrios
    abastecer en un viaje de cisterna única.
    """

    def __init__(self,
                 barrios: List[Barrio],
                 red_vial: RedVialCisterna,
                 capacidad_cisterna: int,
                 radio_maximo_km: float) -> None:
        self.barrios = barrios
        self.red_vial = red_vial
        self.capacidad = capacidad_cisterna
        self.radio_maximo = radio_maximo_km

    def _filtrar_barrios(self, distancias: List[float]) -> List[Tuple[Barrio, float]]:
        """
        Devuelve los barrios candidatos (dentro del radio máximo).
        Cada elemento es (barrio, distancia_desde_planta).
        """
        candidatos: List[Tuple[Barrio, float]] = []
        for barrio in self.barrios:
            d = distancias[barrio.id]
            if d < math.inf and d <= self.radio_maximo and barrio.demanda_litros > 0:
                candidatos.append((barrio, d))
        return candidatos

    def _mochila_01(
        self,
        candidatos: List[Tuple[Barrio, float]]
    ) -> Tuple[int, List[Tuple[Barrio, float]], int]:
        """
        Resuelve el subproblema de mochila 0/1:
        - peso  = demanda_litros
        - valor = personas

        Devuelve: (max_personas, barrios_elegidos, litros_totales)
        """
        k = len(candidatos)
        C = self.capacidad

        # dp[i][c] = max personas usando los primeros i barrios y capacidad c
        dp = [[0] * (C + 1) for _ in range(k + 1)]

        for i in range(1, k + 1):
            barrio, _ = candidatos[i - 1]
            w = barrio.demanda_litros
            val = barrio.personas

            for c in range(C + 1):
                mejor = dp[i - 1][c]  # caso: no tomar el barrio i

                if w <= c:
                    tomar = val + dp[i - 1][c - w]
                    if tomar > mejor:
                        mejor = tomar

                dp[i][c] = mejor

        max_personas = dp[k][C]

        # Reconstrucción de la solución (barrios elegidos)
        elegidos: List[Tuple[Barrio, float]] = []
        c = C
        for i in range(k, 0, -1):
            if dp[i][c] != dp[i - 1][c]:
                barrio, dist_barrio = candidatos[i - 1]
                elegidos.append((barrio, dist_barrio))
                c -= barrio.demanda_litros

        elegidos.reverse()
        litros_totales = sum(b.demanda_litros for b, _ in elegidos)
        return max_personas, elegidos, litros_totales

    def planificar_viaje(
        self
    ) -> Tuple[int, List[Tuple[Barrio, float]], int, List[float]]:
        """
        Flujo completo:
        1) Dijkstra para distancias.
        2) Filtro de barrios por radio.
        3) Mochila 0/1 para maximizar personas.
        4) Orden heurístico de visita (por distancia a la planta).

        Devuelve:
        - max_personas
        - ruta (barrios elegidos + distancia)
        - litros_totales
        - distancias completas (para tablas / resumen)
        """
        # 1. Distancias mínimas
        distancias = self.red_vial.dijkstra_desde_planta()

        # 2. Filtrar por radio
        candidatos = self._filtrar_barrios(distancias)
        if not candidatos:
            return 0, [], 0, distancias

        # 3. Programación Dinámica: Mochila 0/1
        max_personas, elegidos, litros_totales = self._mochila_01(candidatos)

        # 4. Orden sugerido de visita: por distancia creciente
        ruta = sorted(elegidos, key=lambda par: par[1])  # (barrio, distancia)

        return max_personas, ruta, litros_totales, distancias


# ==============================
#  SIMULACIÓN DE EJEMPLO
# ==============================

def main() -> None:
    """
    Simulación básica con datos de ejemplo.
    """

    # 0 = planta de agua
    NODOS = [
        "Planta_Tomilla",
        "Barrio_Cercado",
        "Barrio_Hunter",
        "Barrio_JLByR",
        "Barrio_Yanahuara",
        "Barrio_Paucarpata",
    ]

    # Aristas: (u, v, distancia_en_km)
    ARISTAS = [
        (0, 1, 4.2),
        (0, 2, 6.0),
        (1, 2, 2.5),
        (1, 3, 3.1),
        (2, 4, 4.0),
        (3, 5, 5.5),
        (4, 5, 3.2),
    ]

    red_vial = RedVialCisterna(NODOS, ARISTAS, planta_idx=0)

    barrios = [
        Barrio(id=1, nombre="Barrio_Cercado",   demanda_litros=3000, personas=800),
        Barrio(id=2, nombre="Barrio_Hunter",    demanda_litros=4000, personas=1300),
        Barrio(id=3, nombre="Barrio_JLByR",     demanda_litros=2500, personas=1000),
        Barrio(id=4, nombre="Barrio_Yanahuara", demanda_litros=3500, personas=1600),
        Barrio(id=5, nombre="Barrio_Paucarpata",demanda_litros=5000, personas=2000),
    ]

    CAPACIDAD_CISTERNA = 10000  # litros
    RADIO_MAXIMO = 12.0         # km

    optimizador = OptimizadorCisterna(
        barrios=barrios,
        red_vial=red_vial,
        capacidad_cisterna=CAPACIDAD_CISTERNA,
        radio_maximo_km=RADIO_MAXIMO,
    )

    max_personas, ruta, litros_totales, distancias = optimizador.planificar_viaje()

    # Salida tipo consola (resumen)
    print("====================================================================")
    print("        SISTEMA DE DISTRIBUCIÓN DE AGUA - CISTERNA ÚNICA           ")
    print("====================================================================\n")

    print("[1] RUTAS MÍNIMAS (DIJKSTRA)")
    print("--------------------------------------------------------------------")
    print("Distancias desde la planta:")
    for idx, nombre in enumerate(NODOS):
        print(f"  {nombre:16s} -> {distancias[idx]:5.1f} km")
    print()

    print("[2] PLANIFICACIÓN DEL VIAJE (MOCHILA 0/1)")
    print("--------------------------------------------------------------------")
    if not ruta:
        print("No hay barrios dentro del radio máximo definido.")
        return

    print(f"Capacidad de la cisterna : {CAPACIDAD_CISTERNA} L")
    print(f"Radio máximo considerado : {RADIO_MAXIMO} km\n")

    print(f"Personas máximas abastecidas : {max_personas}")
    print(f"Litros totales utilizados   : {litros_totales} L\n")

    print("Barrios elegidos:")
    for barrio, d in ruta:
        print(
            f"  - {barrio.nombre:16s} | demanda = {barrio.demanda_litros:4d} L"
            f" | personas = {barrio.personas:4d} | dist = {d:4.1f} km"
        )

    print("\nRuta sugerida:")
    nombres_ruta = " -> ".join(
        ["Planta_Tomilla"] + [barrio.nombre for barrio, _ in ruta] + ["Planta_Tomilla"]
    )
    print(" ", nombres_ruta)
    print("====================================================================")


if __name__ == "__main__":
    main()
