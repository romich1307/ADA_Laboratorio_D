import time
import os
from prim import prim_heap_mst
from kruskal import kruskal_mst
from utils import generar_grafo_conectado, edges_set_normalized, dibujar_mst


def main():
    sizes = [10, 50, 100]
    outdir = os.path.join(os.path.dirname(__file__), 'salidas_ej3')
    os.makedirs(outdir, exist_ok=True)

    for n in sizes:
        m = min(n * (n - 1) // 2, 4 * n)
        seed = 42 + n
        V, aristas = generar_grafo_conectado(n, m, seed=seed)
        print('\n' + '=' * 40)
        print(f'Grafo n={n}, m={m}, seed={seed}')

        # Kruskal
        t0 = time.perf_counter()
        mst_k, costo_k, inspeccionadas = kruskal_mst(V, aristas)
        t1 = time.perf_counter()
        tiempo_k = (t1 - t0)

        # Prim (heap)
        t2 = time.perf_counter()
        mst_p, costo_p, pops = prim_heap_mst(V, aristas, inicio=0)
        t3 = time.perf_counter()
        tiempo_p = (t3 - t2)

        print(f'Kruskal: costo={costo_k}, tiempo={tiempo_k:.6f}s, inspeccionadas={inspeccionadas}, aristas_mst={len(mst_k)}')
        print(f'Prim (heap): costo={costo_p}, tiempo={tiempo_p:.6f}s, pops={pops}, aristas_mst={len(mst_p)}')

        same = edges_set_normalized(mst_k) == edges_set_normalized(mst_p)
        print('¿Mismos conjuntos de aristas (considerando peso)?', 'Sí' if same else 'No')

        # Guardar gráficos
        base_name = f'ej3_n{n}'
        dibujar_mst(V, aristas, mst_k, os.path.join(outdir, base_name + '_kruskal_mst.png'), title=f'Kruskal MST n={n}')
        dibujar_mst(V, aristas, mst_p, os.path.join(outdir, base_name + '_prim_mst.png'), title=f'Prim MST n={n}')
        print('Gráficos guardados en', outdir)


if __name__ == '__main__':
    main()
