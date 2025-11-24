#!/usr/bin/env python3
"""
Ejercicio 5: Comparación de Algoritmos de Caminos Mínimos
Floyd-Warshall vs Dijkstra vs Bellman-Ford

Implementación en Python con visualización gráfica de resultados.
"""

import time
import random
import heapq
from typing import List, Tuple, Dict
import matplotlib.pyplot as plt
import numpy as np

INFINITO = float('inf')


# ============================================================================
# CLASE GRAFO
# ============================================================================

class Grafo:
    """Representa un grafo dirigido con matriz de adyacencia y lista de adyacencia."""
    
    def __init__(self, vertices: int):
        self.vertices = vertices
        # Matriz de adyacencia
        self.matriz = [[INFINITO] * vertices for _ in range(vertices)]
        for i in range(vertices):
            self.matriz[i][i] = 0
        
        # Lista de adyacencia: {origen: [(destino, peso), ...]}
        self.lista_adyacencia = [[] for _ in range(vertices)]
    
    def agregar_arista(self, origen: int, destino: int, peso: int):
        """Agrega una arista dirigida al grafo."""
        self.matriz[origen][destino] = peso
        self.lista_adyacencia[origen].append((destino, peso))
    
    def generar_aleatorio(self, densidad: float = 0.3):
        """Genera un grafo aleatorio con la densidad especificada."""
        for i in range(self.vertices):
            for j in range(self.vertices):
                if i != j and random.random() < densidad:
                    peso = random.randint(1, 100)
                    self.agregar_arista(i, j, peso)


# ============================================================================
# ALGORITMO FLOYD-WARSHALL
# ============================================================================

def floyd_warshall(grafo: Grafo) -> Tuple[float, List[List[float]]]:
    """
    Implementa el algoritmo Floyd-Warshall para encontrar todos los caminos más cortos.
    
    Retorna:
        Tupla (tiempo_microsegundos, matriz_distancias)
    """
    inicio = time.perf_counter()
    
    n = grafo.vertices
    # Copiar la matriz de distancias
    dist = [fila[:] for fila in grafo.matriz]
    
    # Algoritmo Floyd-Warshall
    for k in range(n):
        for i in range(n):
            for j in range(n):
                if dist[i][k] != INFINITO and dist[k][j] != INFINITO:
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
    
    fin = time.perf_counter()
    tiempo_us = (fin - inicio) * 1_000_000  # Convertir a microsegundos
    
    return tiempo_us, dist


# ============================================================================
# ALGORITMO DIJKSTRA
# ============================================================================

def dijkstra(grafo: Grafo, origen: int) -> Tuple[float, List[float]]:
    """
    Implementa el algoritmo de Dijkstra para encontrar caminos más cortos desde un origen.
    
    Retorna:
        Tupla (tiempo_microsegundos, lista_distancias)
    """
    inicio = time.perf_counter()
    
    n = grafo.vertices
    dist = [INFINITO] * n
    dist[origen] = 0
    
    # Cola de prioridad: (distancia, nodo)
    pq = [(0, origen)]
    
    while pq:
        d, u = heapq.heappop(pq)
        
        # Si encontramos una distancia obsoleta, la ignoramos
        if d != dist[u]:
            continue
        
        # Relajar aristas adyacentes
        for v, peso in grafo.lista_adyacencia[u]:
            if dist[u] != INFINITO and dist[u] + peso < dist[v]:
                dist[v] = dist[u] + peso
                heapq.heappush(pq, (dist[v], v))
    
    fin = time.perf_counter()
    tiempo_us = (fin - inicio) * 1_000_000
    
    return tiempo_us, dist


# ============================================================================
# ALGORITMO BELLMAN-FORD
# ============================================================================

def bellman_ford(grafo: Grafo, origen: int) -> Tuple[float, List[float]]:
    """
    
    Retorna:
        Tupla (tiempo_microsegundos, lista_distancias)
    """
    inicio = time.perf_counter()
    
    n = grafo.vertices
    dist = [INFINITO] * n
    dist[origen] = 0
    
    # Relajar todas las aristas V-1 veces
    for _ in range(n - 1):
        cambio = False
        for u in range(n):
            if dist[u] == INFINITO:
                continue
            for v, peso in grafo.lista_adyacencia[u]:
                if dist[u] + peso < dist[v]:
                    dist[v] = dist[u] + peso
                    cambio = True
        
        # Optimización: si no hubo cambios, terminar
        if not cambio:
            break
    
    # Detección de ciclos negativos (opcional)
    for u in range(n):
        if dist[u] == INFINITO:
            continue
        for v, peso in grafo.lista_adyacencia[u]:
            if dist[u] + peso < dist[v]:
                # Ciclo negativo detectado
                break
    
    fin = time.perf_counter()
    tiempo_us = (fin - inicio) * 1_000_000
    
    return tiempo_us, dist


# ============================================================================
# FUNCIONES DE BENCHMARKING Y VISUALIZACIÓN
# ============================================================================

def ejecutar_benchmarks(tamanos: List[int], densidad: float = 0.3) -> Dict:
    """
    Ejecuta benchmarks para los tres algoritmos en grafos de diferentes tamaños.
    
    Retorna:
        Diccionario con resultados: {tamano: {algoritmo: tiempo}}
    """
    resultados = {
        'tamanos': tamanos,
        'floyd': [],
        'dijkstra': [],
        'bellman': []
    }
    
    print("=== COMPARACION DE ALGORITMOS DE CAMINOS MINIMOS ===")
    print("Floyd-Warshall vs Dijkstra vs Bellman-Ford\n")
    print(f"{'Vertices':<10} {'Floyd-W (us)':<15} {'Dijkstra (us)':<15} {'Bellman-F (us)':<15}")
    print("-" * 60)
    
    for n in tamanos:
        # Generar grafo aleatorio
        grafo = Grafo(n)
        grafo.generar_aleatorio(densidad)
        
        # Medir Floyd-Warshall
        tiempo_floyd, _ = floyd_warshall(grafo)
        resultados['floyd'].append(tiempo_floyd)
        
        # Medir Dijkstra (desde vértice 0)
        tiempo_dijkstra, _ = dijkstra(grafo, 0)
        resultados['dijkstra'].append(tiempo_dijkstra)
        
        # Medir Bellman-Ford (desde vértice 0)
        tiempo_bellman, _ = bellman_ford(grafo, 0)
        resultados['bellman'].append(tiempo_bellman)
        
        print(f"{n:<10} {tiempo_floyd:<15.2f} {tiempo_dijkstra:<15.2f} {tiempo_bellman:<15.2f}")
    
    return resultados


def graficar_resultados(resultados: Dict):
    """
    Genera gráficos comparativos de los tiempos de ejecución.
    """
    tamanos = resultados['tamanos']
    
    # Crear figura con dos subgráficos
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
    
    # Gráfico 1: Escala lineal
    ax1.plot(tamanos, resultados['floyd'], 'o-', label='Floyd-Warshall', linewidth=2, markersize=8)
    ax1.plot(tamanos, resultados['dijkstra'], 's-', label='Dijkstra', linewidth=2, markersize=8)
    ax1.plot(tamanos, resultados['bellman'], '^-', label='Bellman-Ford', linewidth=2, markersize=8)
    ax1.set_xlabel('Número de Vértices', fontsize=12)
    ax1.set_ylabel('Tiempo de Ejecución (microsegundos)', fontsize=12)
    ax1.set_title('Comparación de Algoritmos - Escala Lineal', fontsize=14, fontweight='bold')
    ax1.legend(fontsize=10)
    ax1.grid(True, alpha=0.3)
    
    # Gráfico 2: Escala logarítmica
    ax2.plot(tamanos, resultados['floyd'], 'o-', label='Floyd-Warshall', linewidth=2, markersize=8)
    ax2.plot(tamanos, resultados['dijkstra'], 's-', label='Dijkstra', linewidth=2, markersize=8)
    ax2.plot(tamanos, resultados['bellman'], '^-', label='Bellman-Ford', linewidth=2, markersize=8)
    ax2.set_xlabel('Número de Vértices', fontsize=12)
    ax2.set_ylabel('Tiempo de Ejecución (microsegundos) - Escala Log', fontsize=12)
    ax2.set_title('Comparación de Algoritmos - Escala Logarítmica', fontsize=14, fontweight='bold')
    ax2.set_yscale('log')
    ax2.legend(fontsize=10)
    ax2.grid(True, alpha=0.3, which='both')
    
    plt.tight_layout()
    
    # Guardar figura
    output_file = 'comparacion_algoritmos.png'
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"\n✓ Grafico guardado como: {output_file}")
    
    # Mostrar gráfico
    plt.show()


def graficar_complejidad_teorica():
    """
    Genera un gráfico comparativo de las complejidades teóricas.
    """
    n = np.linspace(10, 200, 100)
    
    # Complejidades normalizadas (divididas por constantes para visualización)
    floyd = n**3 / 1000  # O(n^3)
    dijkstra = n * np.log(n) * n / 100  # O(n^2 log n) aproximado
    bellman = n**2 * n / 500  # O(n^3) pero con constante menor
    
    plt.figure(figsize=(10, 6))
    plt.plot(n, floyd, label='Floyd-Warshall: O(V³)', linewidth=2)
    plt.plot(n, dijkstra, label='Dijkstra: O((V+E)log V)', linewidth=2)
    plt.plot(n, bellman, label='Bellman-Ford: O(VE)', linewidth=2)
    
    plt.xlabel('Número de Vértices (V)', fontsize=12)
    plt.ylabel('Complejidad Teórica (normalizada)', fontsize=12)
    plt.title('Complejidad Teórica de los Algoritmos', fontsize=14, fontweight='bold')
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    
    output_file = 'complejidad_teorica.png'
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"✓ Grafico de complejidad teorica guardado como: {output_file}")
    plt.show()


def analizar_complejidades():
    """
    Imprime un análisis de las complejidades de los algoritmos.
    """
    print("\n" + "="*70)
    print(" ANALISIS TEORICO DE COMPLEJIDADES")
    print("="*70)
    
    analisis = """
    1. FLOYD-WARSHALL
       - Complejidad temporal: O(V³)
       - Complejidad espacial: O(V²)
       - Encuentra TODOS los caminos mas cortos entre todos los pares de vertices
       - Funciona con pesos negativos (pero no ciclos negativos)
       - Mejor para: grafos densos y cuando necesitamos todos los caminos
    
    2. DIJKSTRA
       - Complejidad temporal: O((V + E) log V) con cola de prioridad
       - Complejidad espacial: O(V + E)
       - Encuentra caminos mas cortos desde UN origen a todos los demas
       - NO funciona con pesos negativos
       - Mejor para: grafos dispersos con pesos no negativos
    
    3. BELLMAN-FORD
       - Complejidad temporal: O(V * E)
       - Complejidad espacial: O(V)
       - Encuentra caminos mas cortos desde UN origen a todos los demas
       - Funciona con pesos negativos y detecta ciclos negativos
       - Mejor para: cuando hay pesos negativos o necesitamos detectar ciclos
    
    OBSERVACIONES:
    - En grafos dispersos (E << V²): Dijkstra es el mas rapido
    - En grafos densos (E ≈ V²): Floyd-Warshall puede ser competitivo si 
      necesitamos todos los caminos
    - Bellman-Ford es mas lento pero mas versatil (pesos negativos)
    """
    print(analisis)
    print("="*70)


# ============================================================================
# FUNCIÓN PRINCIPAL
# ============================================================================

def main():
    """Función principal del programa."""
    print("Iniciando benchmarks...\n")
    
    # Configuración de semilla para reproducibilidad
    random.seed(42)
    
    # Tamaños de grafos a probar (números impares de 1 a 10)
    tamanos = [1,2, 3,4, 5,6,7,8,9,10]
    
    # Ejecutar benchmarks
    resultados = ejecutar_benchmarks(tamanos, densidad=0.3)
    
    # Análisis teórico
    analizar_complejidades()
    
    # Generar gráficos
    print("\nGenerando graficos...")
    graficar_resultados(resultados)
    graficar_complejidad_teorica()
    
    print("\n✓ Proceso completado!")


if __name__ == "__main__":
    main() 