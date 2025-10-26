# Makefile para comparación de algoritmos
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
TARGET = ejercicio4
SOURCE = ejercicio4.cpp

# Compilar
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Limpiar
clean:
	rm -f $(TARGET)

# Ejecutar
run: $(TARGET)
	./$(TARGET)

# Ejecutar con más detalles
test: $(TARGET)
	@echo "=== EJECUTANDO PRUEBAS DE RENDIMIENTO ==="
	./$(TARGET)

.PHONY: all clean run test
