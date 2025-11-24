from collections import defaultdict
import heapq


class TransportRouter:
    def __init__(self):
        self.graph = defaultdict(list)
        
    def add_edge(self, from_node, to_node, weight, mode):
        """Agregar una arista (camino) entre dos nodos (lugares), con un peso (tiempo/costo) y modo de transporte."""
        self.graph[from_node].append((to_node, weight, mode))
    
    def dijkstra(self, start, end, preference="tiempo"):
        """Ejecuta el algoritmo de Dijkstra para encontrar la ruta mas corta segun la preferencia."""
        pq = [(0, start, [])]  # (tiempo, nodo actual, ruta tomada)
        distances = {start: 0}
        visited = set()

        while pq:
            current_distance, current_node, path = heapq.heappop(pq)

            if current_node in visited:
                continue

            visited.add(current_node)
            path = path + [current_node]

            if current_node == end:
                return current_distance, path  # Retorna el tiempo y la ruta

            for neighbor, weight, mode in self.graph[current_node]:
                if neighbor in visited:
                    continue
                new_distance = current_distance + weight
                if neighbor not in distances or new_distance < distances[neighbor]:
                    distances[neighbor] = new_distance
                    heapq.heappush(pq, (new_distance, neighbor, path))
        
        return float("inf"), []  # Si no hay ruta

    def recomendar(self, start, end, preference="tiempo"):
        """Calcula las opciones segun la preferencia del usuario: tiempo, costo o equilibrio."""
        if preference == "tiempo":
            return self.dijkstra(start, end, preference="tiempo")
        elif preference == "costo":
            return self.dijkstra(start, end, preference="costo")
        elif preference == "equilibrio":
            time_result = self.dijkstra(start, end, preference="tiempo")
            cost_result = self.dijkstra(start, end, preference="costo")
            return min(time_result, cost_result, key=lambda x: x[0])
        return self.dijkstra(start, end, preference="tiempo")


if __name__ == '__main__':
    router = TransportRouter()
    
    # Definir las conexiones (Auto)
    router.add_edge("Terminal", "Cercado", 12.8, "auto")
    router.add_edge("Cercado", "Plaza", 3.8, "auto")
    
    # Definir las conexiones (Bus)
    router.add_edge("Terminal", "Cercado", 26.9, "bus")
    router.add_edge("Cercado", "Plaza", 5.0, "bus")
    
    resultado = router.recomendar('Terminal', 'Plaza', preference='equilibrio')
    print(f"Resultado: Tiempo {resultado[0]:.1f} min, Ruta: {' -> '.join(resultado[1])}")


