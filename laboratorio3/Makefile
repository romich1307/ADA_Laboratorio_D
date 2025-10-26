# Makefile para el Sistema de Gestión de Estudiantes

# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g

# Nombre del ejecutable
TARGET = estudiantes

# Archivos fuente
SOURCES = main.cpp Estudiante.cpp MergeSort.cpp GestorEstudiantes.cpp

# Archivos objeto (se generan automáticamente)
OBJECTS = $(SOURCES:.cpp=.o)

# Archivos de cabecera
HEADERS = Estudiante.h MergeSort.h GestorEstudiantes.h

# Regla principal
all: $(TARGET)

# Crear el ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "¡Compilación exitosa! Ejecute con: ./$(TARGET)"

# Compilar archivos objeto
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Archivos limpiados."

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Compilar en modo debug
debug: CXXFLAGS += -DDEBUG
debug: $(TARGET)

# Compilar optimizado para producción
release: CXXFLAGS += -O3 -DNDEBUG
release: clean $(TARGET)

# Verificar sintaxis sin compilar
check:
	$(CXX) $(CXXFLAGS) -fsyntax-only $(SOURCES)
	@echo "Verificación de sintaxis completada."

# Mostrar ayuda
help:
	@echo "Uso del Makefile:"
	@echo "  make         - Compilar el proyecto"
	@echo "  make clean   - Limpiar archivos generados"
	@echo "  make run     - Compilar y ejecutar"
	@echo "  make debug   - Compilar con información de debug"
	@echo "  make release - Compilar optimizado para producción"
	@echo "  make check   - Verificar sintaxis sin compilar"
	@echo "  make help    - Mostrar esta ayuda"

# Declarar targets que no son archivos
.PHONY: all clean run debug release check help
