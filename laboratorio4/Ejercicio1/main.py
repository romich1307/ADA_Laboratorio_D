
from ciclos import Ciclos
from kruskal import kruskals_mst


def ejemplo():
    # grafo con 4 vértices y algunas aristas
    V = list(range(4))
    aristas = [
        (0, 1, 1),
        (0, 2, 5),
        (1, 2, 2),
        (1, 3, 4),
        (2, 3, 1),
    ]

    costo = kruskals_mst(V, aristas)
    print(f"Costo MST ejemplo: {costo}")


if __name__ == "__main__":
    ejemplo()
