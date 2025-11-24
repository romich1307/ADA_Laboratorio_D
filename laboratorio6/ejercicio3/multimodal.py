from __future__ import annotations
from dataclasses import dataclass
from enum import Enum
import heapq
from typing import Dict, List, Tuple, Optional


class TransportMode(Enum):
    AUTO = "auto"
    BUS = "bus"
    TAXI = "taxi"
    WALK = "walk"


@dataclass
class Edge:
    to_node: str
    distance_km: float
    stops: int = 0  # for buses: number of intermediate stops on this edge


class Graph:
    def __init__(self, mode: TransportMode):
        self.mode = mode
        self.adj: Dict[str, List[Edge]] = {}
        # per-node extra info e.g. parking penalty in minutes
        self.node_parking_penalty: Dict[str, float] = {}
        # congestion factor per edge approximated per-node here
        self.congestion: Dict[Tuple[str, str], float] = {}
        # bus average wait time (minutes)
        self.bus_wait_minutes: float = 8.0

    def add_node(self, name: str, parking_penalty_min: float = 0.0):
        self.adj.setdefault(name, [])
        if parking_penalty_min:
            self.node_parking_penalty[name] = parking_penalty_min

    def add_edge(self, a: str, b: str, distance_km: float, stops: int = 0):
        self.adj.setdefault(a, []).append(Edge(b, distance_km, stops))
        self.adj.setdefault(b, []).append(Edge(a, distance_km, stops))
        self.congestion[(a, b)] = 1.0
        self.congestion[(b, a)] = 1.0

    def set_congestion(self, a: str, b: str, factor: float):
        if (a, b) in self.congestion:
            self.congestion[(a, b)] = factor
            self.congestion[(b, a)] = factor

    def neighbors(self, node: str) -> List[Edge]:
        return self.adj.get(node, [])


def dijkstra(graph: Graph, origin: str, destination: str) -> Optional[Tuple[float, float, List[str]]]:
    # Returns (time_minutes, cost_soles, path_nodes) minimizing time for that transport mode
    if origin not in graph.adj or destination not in graph.adj:
        return None

    INF = float('inf')
    dist_time: Dict[str, float] = {n: INF for n in graph.adj}
    dist_cost: Dict[str, float] = {n: INF for n in graph.adj}
    prev: Dict[str, Optional[str]] = {n: None for n in graph.adj}

    dist_time[origin] = 0.0
    dist_cost[origin] = 0.0
    heap = [(0.0, origin)]  # (time, node)

    while heap:
        time_so_far, u = heapq.heappop(heap)
        if time_so_far > dist_time[u]:
            continue
        if u == destination:
            break
        for e in graph.neighbors(u):
            # compute time and cost for this edge depending on transport
            cong = graph.congestion.get((u, e.to_node), 1.0)
            if graph.mode == TransportMode.AUTO:
                minutes = (e.distance_km / 40.0) * 60.0 * cong
                cost = e.distance_km * 0.50
                # add parking penalty only when arriving to destination node
                penalty = graph.node_parking_penalty.get(e.to_node, 0.0)
                minutes += penalty
            elif graph.mode == TransportMode.BUS:
                # bus: distance travel time + stops * 0.5 min + waiting time (add only once at origin)
                minutes = (e.distance_km / 30.0) * 60.0 + (e.stops * 0.5)
                cost = 1.50  # flat fare per trip (we'll count once per origin->destination path)
                penalty = 0.0
            elif graph.mode == TransportMode.TAXI:
                minutes = (e.distance_km / 45.0) * 60.0 * cong
                cost = 5.0 + 1.5 * e.distance_km
                penalty = 0.0
            else:  # walk
                minutes = (e.distance_km / 5.0) * 60.0
                cost = 0.0
                penalty = 0.0

            next_time = time_so_far + minutes
            next_cost = dist_cost[u] + cost
            # For bus, if we are departing from origin, add wait time once
            if graph.mode == TransportMode.BUS and u == origin:
                next_time += graph.bus_wait_minutes

            if next_time < dist_time[e.to_node]:
                dist_time[e.to_node] = next_time
                dist_cost[e.to_node] = next_cost
                prev[e.to_node] = u
                heapq.heappush(heap, (next_time, e.to_node))

    if dist_time[destination] == INF:
        return None

    # reconstruct path
    path = []
    cur = destination
    while cur is not None:
        path.append(cur)
        cur = prev[cur]
    path.reverse()
    # For bus we treated cost as sum of edge fares; convert to single fare if >0
    if graph.mode == TransportMode.BUS and dist_cost[destination] > 0:
        dist_cost[destination] = 1.50

    return dist_time[destination], dist_cost[destination], path


class MultimodalRouter:
    def __init__(self):
        self.auto_graph = Graph(TransportMode.AUTO)
        self.bus_graph = Graph(TransportMode.BUS)
        self.taxi_graph = Graph(TransportMode.TAXI)
        self._build_example_network()

    def _build_example_network(self):
        # Nodes: Terminal Terrestre, AvEjercito, PuenteGrau, Plaza, Cercado
        nodes = ["Terminal", "AvEjercito", "PuenteGrau", "Plaza", "Cercado"]
        for n in nodes:
            self.auto_graph.add_node(n, parking_penalty_min=2.0 if n == "Plaza" else 0.0)
            self.taxi_graph.add_node(n)
        # Bus graph uses bus stops (subset)
        for n in nodes:
            self.bus_graph.add_node(n)

        # Add edges (distances approximate, km)
        edges = [
            ("Terminal", "AvEjercito", 4.0, 3),
            ("AvEjercito", "PuenteGrau", 2.5, 4),
            ("PuenteGrau", "Plaza", 1.0, 2),
            ("AvEjercito", "Plaza", 3.5, 5),
            ("Terminal", "Cercado", 5.0, 6),
            ("Cercado", "Plaza", 2.2, 3),
        ]
        for a, b, d, stops in edges:
            self.auto_graph.add_edge(a, b, d)
            self.taxi_graph.add_edge(a, b, d)
            self.bus_graph.add_edge(a, b, d, stops=stops)

        # Example congestion: center streets more congested for autos
        self.auto_graph.set_congestion("AvEjercito", "PuenteGrau", 1.5)
        self.auto_graph.set_congestion("PuenteGrau", "Plaza", 2.0)
        # Taxis know shortcuts (no extra congestion on AvEjercito)
        self.taxi_graph.set_congestion("AvEjercito", "PuenteGrau", 1.1)
        self.taxi_graph.set_congestion("PuenteGrau", "Plaza", 1.0)

    def compute_mode_route(self, mode: TransportMode, origin: str, dest: str):
        if mode == TransportMode.AUTO:
            return dijkstra(self.auto_graph, origin, dest)
        if mode == TransportMode.BUS:
            return dijkstra(self.bus_graph, origin, dest)
        if mode == TransportMode.TAXI:
            return dijkstra(self.taxi_graph, origin, dest)
        return None

    def recommend(self, origin: str, dest: str, preference: str = "equilibrio") -> Dict:
        # preference: 'tiempo'|'costo'|'comodidad'|'equilibrio'
        candidates = []

        # 1) Single-mode options
        for mode in (TransportMode.AUTO, TransportMode.BUS, TransportMode.TAXI):
            r = self.compute_mode_route(mode, origin, dest)
            if r:
                time_min, cost, path = r
                candidates.append({
                    "modes": [mode.value],
                    "time_min": time_min,
                    "cost": cost,
                    "path": path,
                    "comfort": self._comfort_score(mode),
                })

        # 2) Simple 2-mode combos: bus -> taxi, bus -> walk
        # consider transfer nodes that exist in both graphs
        transfer_nodes = set(self.bus_graph.adj.keys()) & set(self.taxi_graph.adj.keys())
        for transfer in transfer_nodes:
            # bus -> taxi
            first = dijkstra(self.bus_graph, origin, transfer)
            second = dijkstra(self.taxi_graph, transfer, dest)
            if first and second:
                time = first[0] + second[0] + 0.0  # taxi wait negligible or small
                cost = first[1] + second[1]
                path = first[2][:-1] + second[2]
                candidates.append({
                    "modes": [TransportMode.BUS.value, TransportMode.TAXI.value],
                    "time_min": time,
                    "cost": cost,
                    "path": path,
                    "comfort": (self._comfort_score(TransportMode.BUS) + self._comfort_score(TransportMode.TAXI)) / 2,
                })

        # scoring: normalize and choose best according to preference
        best = self._choose_best(candidates, preference)
        return {"options": candidates, "best": best}

    def _comfort_score(self, mode: TransportMode) -> float:
        if mode == TransportMode.TAXI:
            return 1.0
        if mode == TransportMode.AUTO:
            return 0.7
        if mode == TransportMode.BUS:
            return 0.3
        return 0.5

    def _choose_best(self, candidates: List[Dict], preference: str) -> Optional[Dict]:
        if not candidates:
            return None
        # find max time and cost to normalize
        max_time = max(c['time_min'] for c in candidates)
        max_cost = max(c['cost'] for c in candidates)

        def score(c):
            t = c['time_min'] / max_time if max_time > 0 else 0
            p = c['cost'] / max_cost if max_cost > 0 else 0
            comfort = c['comfort']
            if preference == 'tiempo':
                return t * 0.9 + p * 0.1 - comfort * 0.05
            if preference == 'costo':
                return p * 0.9 + t * 0.1 - comfort * 0.05
            if preference == 'comodidad':
                return -comfort  # higher comfort is better
            # equilibrio
            return 0.5 * t + 0.5 * p - 0.1 * comfort

        # minimize score
        best = min(candidates, key=score)
        best['score'] = score(best)
        return best


if __name__ == '__main__':
    router = MultimodalRouter()
    res = router.recommend('Terminal', 'Plaza', preference='equilibrio')
    print('Opciones generadas:')
    for o in res['options']:
        print(f"- Modos: {o['modes']}, Tiempo (min): {o['time_min']:.1f}, Costo: S/.{o['cost']:.2f}, Ruta: {o['path']}")
    print('\nRecomendación:')
    b = res['best']
    if b:
        print(f"Mejor opción: {b['modes']} → Tiempo {b['time_min']:.1f} min, Costo S/.{b['cost']:.2f}, Ruta: {b['path']}")
