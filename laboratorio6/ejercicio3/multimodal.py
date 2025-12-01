from collections import defaultdict
import heapq


class TransportRouter:
    def __init__(self):
        self.graph = defaultdict(list)
        
    def add_edge(self, from_node, to_node, weight, mode):
        """Agregar una arista (camino) entre dos nodos (lugares), con un peso (tiempo/costo) y modo de transporte."""
        self.graph[from_node].append((to_node, weight, mode))
    
    def dijkstra(self, start, end, mode_filter=None):
        """
        Algoritmo de Dijkstra para encontrar la ruta mas corta.
        mode_filter: si se especifica, solo considera aristas de ese modo ('auto' o 'bus')
        """
        pq = [(0, start, [], None)]  # (peso, nodo actual, ruta tomada, modo usado)
        distances = {start: 0}
        visited = set()

        while pq:
            current_distance, current_node, path, mode_used = heapq.heappop(pq)

            if current_node in visited:
                continue

            visited.add(current_node)
            path = path + [current_node]

            if current_node == end:
                return current_distance, path, mode_used

            for neighbor, weight, mode in self.graph[current_node]:
                # Filtrar por modo si se especifica
                if mode_filter and mode != mode_filter:
                    continue
                    
                if neighbor in visited:
                    continue
                new_distance = current_distance + weight
                if neighbor not in distances or new_distance < distances[neighbor]:
                    distances[neighbor] = new_distance
                    heapq.heappush(pq, (new_distance, neighbor, path, mode))
        
        return float("inf"), [], None

    def programacion_dinamica_comparar(self, start, end):
        """
        Programacion Dinamica para comparar opciones de transporte.
        Calcula rutas para Auto y Bus, luego usa DP para elegir la mejor combinacion.
        """
        # Calcular rutas por cada modo usando Dijkstra
        tiempo_auto, ruta_auto, _ = self.dijkstra(start, end, mode_filter='auto')
        tiempo_bus, ruta_bus, _ = self.dijkstra(start, end, mode_filter='bus')
        
        # Definir costos (estimados)
        costo_auto = tiempo_auto * 0.5  # S/. 0.50 por minuto aproximado
        costo_bus = 1.50  # Tarifa fija
        
        # Tabla DP: dp[opcion] = (tiempo, costo, ruta, modo)
        # Estado: evaluar todas las combinaciones posibles
        opciones = []
        
        if tiempo_auto < float("inf"):
            opciones.append({
                'modo': 'Auto',
                'tiempo': tiempo_auto,
                'costo': costo_auto,
                'ruta': ruta_auto,
                'score_tiempo': tiempo_auto,
                'score_costo': costo_auto
            })
        
        if tiempo_bus < float("inf"):
            opciones.append({
                'modo': 'Bus',
                'tiempo': tiempo_bus,
                'costo': costo_bus,
                'ruta': ruta_bus,
                'score_tiempo': tiempo_bus,
                'score_costo': costo_bus
            })
        
        return opciones

    def recomendar(self, start, end, preference="tiempo"):
        """
        Usa Dijkstra + Programacion Dinamica para elegir la mejor opcion.
        1. Dijkstra: calcula rutas mas cortas para cada modo
        2. DP: compara y elige segun preferencia del usuario
        """
        # Paso 1: Obtener opciones usando DP
        opciones = self.programacion_dinamica_comparar(start, end)
        
        if not opciones:
            return float("inf"), []
        
        # Paso 2: DP - elegir mejor opcion segun preferencia
        if preference == "tiempo":
            # Minimizar tiempo
            mejor = min(opciones, key=lambda x: x['score_tiempo'])
        elif preference == "costo":
            # Minimizar costo
            mejor = min(opciones, key=lambda x: x['score_costo'])
        elif preference == "equilibrio":
            # DP: normalizar y balancear (50% tiempo + 50% costo)
            max_tiempo = max(o['tiempo'] for o in opciones)
            max_costo = max(o['costo'] for o in opciones)
            
            for opc in opciones:
                t_norm = opc['tiempo'] / max_tiempo if max_tiempo > 0 else 0
                c_norm = opc['costo'] / max_costo if max_costo > 0 else 0
                opc['score_equilibrio'] = 0.5 * t_norm + 0.5 * c_norm
            
            mejor = min(opciones, key=lambda x: x['score_equilibrio'])
        else:
            mejor = opciones[0]
        
        return mejor['tiempo'], mejor['ruta']


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


