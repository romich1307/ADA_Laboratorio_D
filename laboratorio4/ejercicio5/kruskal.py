class DisjointSet:
    def __init__(self, n):
        self.parent = list(range(n))
        self.rank = [0] * n

    def find(self, x):
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]

    def union(self, x, y):
        rx = self.find(x)
        ry = self.find(y)
        if rx == ry:
            return False
        if self.rank[rx] < self.rank[ry]:
            self.parent[rx] = ry
        elif self.rank[ry] < self.rank[rx]:
            self.parent[ry] = rx
        else:
            self.parent[ry] = rx
            self.rank[rx] += 1
        return True


def kruskal_mst(V, aristas):
    ar_sorted = sorted(aristas, key=lambda x: x[2])
    dsu = DisjointSet(len(V))
    mst = []
    inspeccionadas = 0
    for u, v, w in ar_sorted:
        inspeccionadas += 1
        if dsu.find(u) != dsu.find(v):
            dsu.union(u, v)
            mst.append((u, v, w))
            if len(mst) == len(V) - 1:
                break
    costo = sum(w for (_, _, w) in mst)
    return mst, costo, inspeccionadas
