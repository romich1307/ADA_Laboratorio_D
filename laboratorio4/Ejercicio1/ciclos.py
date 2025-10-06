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
        
