from multimodal import MultimodalRouter


def main():
    router = MultimodalRouter()
    print('== SISTEMA DE NAVEGACIÓN MULTIMODAL - Ejemplo: Terminal → Plaza ==')
    for pref in ['tiempo', 'costo', 'comodidad', 'equilibrio']:
        print(f'\n-- Preferencia: {pref} --')
        res = router.recommend('Terminal', 'Plaza', preference=pref)
        for o in res['options']:
            print(f"Opc: {o['modes']}: Tiempo {o['time_min']:.1f} min - Costo S/.{o['cost']:.2f} - Ruta: {o['path']}")
        best = res['best']
        if best:
            print(f"Recomendado: {best['modes']} → {best['time_min']:.1f} min, S/.{best['cost']:.2f}")


if __name__ == '__main__':
    main()
