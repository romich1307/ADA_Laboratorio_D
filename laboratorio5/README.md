# Comparación de Algoritmos de Caminos Mínimos

## Descripción
Este programa compara el rendimiento de tres algoritmos fundamentales para encontrar caminos mínimos:
- **Floyd-Warshall** (ya implementado)
- **Dijkstra** (pendiente de implementar)
- **Bellman-Ford** (pendiente de implementar)

## Estado del Proyecto
- ✅ **Floyd-Warshall**: Completamente implementado
- ⏳ **Dijkstra**: Esqueleto creado, implementación pendiente
- ⏳ **Bellman-Ford**: Esqueleto creado, implementación pendiente

## Para Tu Compañera: Qué Implementar

### 1. Algoritmo Dijkstra (líneas ~60-80)
```cpp
// Buscar en el código: "TODO: IMPLEMENTAR ALGORITMO DIJKSTRA AQUÍ"
```
**Pasos a implementar:**
1. Usar `priority_queue` para cola de prioridad
2. Inicializar `dist[origen] = 0`
3. Mientras la cola no esté vacía:
   - Extraer vértice con menor distancia
   - Para cada vecino, relajar la arista si es mejor

### 2. Algoritmo Bellman-Ford (líneas ~90-110)
```cpp
// Buscar en el código: "TODO: IMPLEMENTAR ALGORITMO BELLMAN-FORD AQUÍ"
```
**Pasos a implementar:**
1. Inicializar `dist[origen] = 0`
2. Repetir V-1 veces:
   - Para cada arista (u,v) con peso w:
   - Si `dist[u] + w < dist[v]`: actualizar `dist[v] = dist[u] + w`

## Cómo Compilar y Ejecutar
```bash
# Compilar
make

# Ejecutar
make run

# Ejecutar con información detallada
make test

# Limpiar archivos compilados
make clean
```

## Estructura de Archivos
- `ejercicio4.cpp` - Código principal con comparación
- `Makefile` - Para compilar fácilmente
- `README.md` - Este archivo con instrucciones

## Resultados Esperados
El programa mostrará:
1. Tabla comparativa de tiempos de ejecución
2. Análisis teórico de complejidades
3. Recomendaciones de uso para cada algoritmo

## Notas Importantes
- Floyd-Warshall calcula **TODOS** los caminos entre **TODOS** los pares
- Dijkstra y Bellman-Ford calculan caminos desde **UN** origen específico
- Para comparación justa, habría que ejecutar Dijkstra V veces
- Los tiempos pueden variar según el hardware y tamaño del grafo

¡Buena suerte con la implementación! 🚀
