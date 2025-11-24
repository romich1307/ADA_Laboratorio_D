#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>

using namespace std;

int main(){
    int Vertices = 4;
    int Aristas = 6;

    int Grafo[Vertices][Vertices];

    for(int i=0;i< Vertices;i++){
        for(int j=0;j<Vertices;j++){
            Grafo[i][j] = 0;
        }
    }

    // Generar aristas aleatorias
    srand(time(NULL));
    for(int i=0;i<Aristas;i++){
        int u = rand() % Vertices;
        int v = rand() % Vertices;
        int peso = rand() % 10 + 1; 
        if(u != v && Grafo[u][v] == 0){
            Grafo[u][v] = peso;
            Grafo[v][u] = peso;  
        }
    }

    // Imprimir el grafo
    printf("Grafo generado:\n");
    for(int i=0;i<Vertices;i++){
        for(int j=0;j<Vertices;j++){
            if(Grafo[i][j] != 0){
                printf("Arista: %d - %d, Peso: %d\n", i, j, Grafo[i][j]);
            }
        }
    }
    //Ingresamos la arista inicial para hallar el camino mas corto a cada nodo
    int verticeInicial;
    cout<<"Ingrese el vertice inicial: ";
    cin>>verticeInicial;

    // Hallamos la distancia de cada nodo desde el nodo inicial
    for(int i=0;i<Vertices;i++){
        if(Grafo[verticeInicial][i] != 0){
            cout<<"La distancia desde el vertice "<<verticeInicial<<" hasta el vertice "<<i<<" es: "<<Grafo[verticeInicial][i]<<endl;
        }
    }


    return 0;
}