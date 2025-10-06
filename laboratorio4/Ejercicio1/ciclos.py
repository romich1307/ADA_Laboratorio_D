#Implementa	Prim	y	Kruskal	para	un	grafo	con	6	vértices	y	
# 10	aristas	aleatorias	y	compara	el	
#número	de	aristas	procesadas	por	cada	algoritmo	
# (El	algoritmo	de	Prim	comienzenlo	desde	un	
#vértice	elegido	por	ustedes).	
class ciclos:
    def _init_(objeto,cantidad):
        objeto.padre=list(range(cantidad))
        objeto.rango=[1]*cantidad
    
    def encontrar(objeto, i):
        if objeto.padre[i] !=i:
            objeto.padre[i]=objeto.encontar(objeto.padre[i]) 
        return objeto.padre[i]
    def union(objeto,nodo_u,nodo_v):
        raiz_u=objeto.encontrar(nodo_u)
        raiz_v=objeto.encontrar(nodo_v)
        if raiz_u !=raiz_v:
            if objeto.rango[raiz_u]>objeto.rango[raiz_v]:
                objeto.padre[raiz_v]=raiz_u
            elif objeto.rango[raiz_u]<objeto.rango[raiz_v]:
                objeto.padre[raiz_u]=raiz_v
            else:
                objeto.padre[raiz_v]=raiz_u
                objeto.rango[raiz_u]+=1
