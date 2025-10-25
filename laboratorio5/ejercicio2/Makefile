# Makefile simple para Floyd-Warshall
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
TARGET = ejercicio2
SOURCE = ejercicio2.cpp

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

.PHONY: all clean run
