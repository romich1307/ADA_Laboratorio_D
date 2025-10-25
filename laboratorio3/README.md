# Sistema de Gestión de Estudiantes con MergeSort

Este proyecto implementa un sistema de gestión de estudiantes que utiliza el algoritmo **MergeSort** para ordenar registros de estudiantes por su promedio ponderado.

## Estructura del Proyecto

El proyecto está organizado en las siguientes clases separadas:

### 1. Clase `Estudiante` (`Estudiante.h` / `Estudiante.cpp`)
- **Propósito**: Representa un estudiante con sus atributos básicos
- **Atributos**:
  - `codigo` (string): Código único del estudiante
  - `nombre` (string): Nombre completo del estudiante
  - `promedioPonderado` (float): Promedio ponderado del estudiante
- **Funcionalidades**:
  - Constructores por defecto y con parámetros
  - Getters y setters para todos los atributos
  - Operadores de comparación (`<`, `>`, `<=`, `>=`)
  - Función para mostrar información
  - Sobrecarga del operador `<<` para impresión

### 2. Clase `MergeSort` (`MergeSort.h` / `MergeSort.cpp`)
- **Propósito**: Implementa el algoritmo MergeSort para ordenar vectores de estudiantes
- **Funcionalidades**:
  - `ordenar()`: Ordena en forma ascendente por promedio ponderado
  - `ordenarDescendente()`: Ordena en forma descendente por promedio ponderado
  - Funciones auxiliares privadas para la implementación recursiva
  - **Complejidad temporal**: O(n log n)
  - **Complejidad espacial**: O(n)

### 3. Clase `GestorEstudiantes` (`GestorEstudiantes.h` / `GestorEstudiantes.cpp`)
- **Propósito**: Gestiona la colección de estudiantes y las operaciones sobre ella
- **Funcionalidades**:
  - Agregar estudiantes individual o en lote
  - Mostrar lista de estudiantes
  - Interfaz para ordenamiento usando MergeSort
  - Validación de entrada de datos
  - Gestión de la lista (limpiar, obtener estadísticas)

### 4. Programa Principal (`main.cpp`)
- **Propósito**: Interfaz de usuario con menú interactivo
- **Funcionalidades**:
  - Menú principal con 7 opciones
  - Ingreso de estudiantes desde consola
  - Visualización de estudiantes y estadísticas
  - Ordenamiento ascendente y descendente
  - Gestión de errores en entrada de datos

## Características del MergeSort Implementado

### Algoritmo
- **Paradigma**: Divide y vencerás
- **Estabilidad**: Algoritmo estable (mantiene el orden relativo de elementos iguales)
- **Complejidad temporal**: O(n log n) en todos los casos (mejor, promedio y peor)
- **Complejidad espacial**: O(n) por el uso de vectores auxiliares

### Ventajas de MergeSort
1. **Rendimiento consistente**: O(n log n) en todos los casos
2. **Estable**: Preserva el orden de elementos con valores iguales
3. **Predecible**: No depende de la distribución inicial de los datos
4. **Paralelizable**: Se puede adaptar para procesamiento paralelo

## Compilación y Ejecución

### Compilar el proyecto:
```bash
g++ -o estudiantes main.cpp Estudiante.cpp MergeSort.cpp GestorEstudiantes.cpp
```

### Ejecutar:
```bash
./estudiantes
```

## Uso del Sistema

1. **Ingresar estudiantes**: Permite ingresar múltiples estudiantes con validación de datos
2. **Mostrar estudiantes**: Muestra la lista actual con estadísticas
3. **Ordenar ascendente**: Ordena de menor a mayor promedio usando MergeSort
4. **Ordenar descendente**: Ordena de mayor a menor promedio usando MergeSort
5. **Agregar estudiante individual**: Agrega un solo estudiante
6. **Limpiar lista**: Elimina todos los estudiantes de la lista
7. **Salir**: Termina el programa

## Validaciones Implementadas

- **Promedio ponderado**: Debe estar entre 0.0 y 20.0
- **Entradas numéricas**: Validación de tipo de dato
- **Entradas vacías**: Manejo de strings vacíos
- **Opciones de menú**: Validación de rango de opciones

## Ejemplo de Uso

```
Código: EST001
Nombre: Juan Pérez
Promedio: 15.75

Código: EST002
Nombre: María García
Promedio: 18.20

Código: EST003
Nombre: Carlos López
Promedio: 12.50
```

**Resultado ordenado ascendente:**
1. Carlos López - 12.50
2. Juan Pérez - 15.75  
3. María García - 18.20

## Complejidad del Sistema

- **Inserción**: O(1) amortizado
- **Ordenamiento**: O(n log n)
- **Búsqueda/Mostrar**: O(n)
- **Espacio**: O(n) para almacenar n estudiantes

## Extensiones Posibles

- Ordenamiento por otros criterios (nombre, código)
- Persistencia de datos (archivos)
- Búsqueda de estudiantes
- Interfaz gráfica
- Validación más robusta de códigos únicos
