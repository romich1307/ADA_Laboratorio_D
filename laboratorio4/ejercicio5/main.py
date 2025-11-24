import os
import time
from utils import leer_red_desde_txt, dibujar_red
from prim import prim_heap_mst
from kruskal import kruskal_mst


def procesar_entrada(path):
    V, aristas = leer_red_desde_txt(path)
    print('Procesando', os.path.basename(path))
    mst_k, costo_k, inspeccionadas = kruskal_mst(V, aristas)
    mst_p, costo_p, pops = prim_heap_mst(V, aristas)
    print(f'Kruskal: costo={costo_k}, inspeccionadas={inspeccionadas}')
    print(f'Prim (heap): costo={costo_p}, pops={pops}')
    same = set(((min(a,b), max(a,b), w) for (a,b,w) in mst_k)) == set(((min(a,b), max(a,b), w) for (a,b,w) in mst_p))
    print('¿Mismo MST? ', 'Sí' if same else 'No')
    outdir = os.path.join(os.path.dirname(path), '..', 'salidas_ej5')
    os.makedirs(outdir, exist_ok=True)
    base = os.path.splitext(os.path.basename(path))[0]
    # si existe un archivo con nombre en español equivalente, prefierelo
    entradas_dir = os.path.join(os.path.dirname(__file__), 'entradas')
    spanish_map = {
        'ambiguous_cycle4': 'ciclo_ambiguo4',
        'ambiguous_grid': 'malla_ambiguo',
        'ambiguous_two_mst': 'dos_mst_ambiguo',
        'unique_weights': 'pesos_unicos',
        'sparse_vs_dense': 'disperso_vs_denso',
        'red_pequena': 'red_pequena'
    }
    base_spanish = spanish_map.get(base, base)
    # si el fichero español existe, úsalo como base de nombres
    spanish_path = os.path.join(entradas_dir, base_spanish + '.txt')
    if os.path.exists(spanish_path):
        base_to_use = base_spanish
    else:
        base_to_use = base
    title_k = f"Red de computadoras de la UNSA — {base_to_use} (Kruskal)"
    title_p = f"Red de computadoras de la UNSA — {base_to_use} (Prim)"
    dibujar_red(V, aristas, mst_k, os.path.join(outdir, base_to_use + '_kruskal.png'), title=title_k)
    dibujar_red(V, aristas, mst_p, os.path.join(outdir, base_to_use + '_prim.png'), title=title_p)
    print('Gráficos guardados en', outdir)


if __name__ == '__main__':
    entradas_dir = os.path.join(os.path.dirname(__file__), 'entradas')
    archivos = [os.path.join(entradas_dir, f) for f in os.listdir(entradas_dir) if f.endswith('.txt')]
    for a in archivos:
        try:
            procesar_entrada(a)
        except ValueError as e:
            print(f"Archivo {os.path.basename(a)} omitido: formato inválido ({e})")
            continue
