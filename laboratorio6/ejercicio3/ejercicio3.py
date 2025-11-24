from multimodal import TransportRouter


def main():
    print("=" * 70)
    print("   SISTEMA DE NAVEGACION URBANA INTELIGENTE - AREQUIPA")
    print("=" * 70)
    print("\nObjetivo: Encontrar la mejor ruta entre dos puntos de la ciudad")
    print("Modos de transporte: Auto y Bus")
    print("Algoritmo: Dijkstra para calcular rutas mas cortas\n")
    
    router = TransportRouter()
    
    # Definir las conexiones y los costos (tiempo en minutos)
    # Auto (mas rapido)
    router.add_edge("Terminal", "Cercado", 12.8, "auto")
    router.add_edge("Cercado", "Plaza", 3.8, "auto")
    
    # Bus (mas lento pero economico)
    router.add_edge("Terminal", "Cercado", 26.9, "bus")
    router.add_edge("Cercado", "Plaza", 5.0, "bus")

    origen = "Terminal"
    destino = "Plaza"
    
    print(f"Ruta solicitada: {origen} -> {destino}\n")
    print("=" * 70)
    
    # Probar las 3 preferencias principales
    preferencias = [
        ("tiempo", "Prioridad: RAPIDEZ (minimizar tiempo de viaje)"),
        ("costo", "Prioridad: ECONOMIA (minimizar gasto)"),
        ("equilibrio", "Prioridad: BALANCE (50% tiempo + 50% costo)")
    ]
    
    for pref, descripcion in preferencias:
        print(f"\n{'-' * 70}")
        print(f"CASO: {descripcion}")
        print('-' * 70)
        
        tiempo, ruta = router.recomendar(origen, destino, preference=pref)
        
        print("\nOpciones calculadas:\n")
        modo_usado = "Auto" if tiempo < 20 else "Bus"
        print(f"Modo: {modo_usado}")
        print(f"   - Tiempo total: {tiempo:.1f} minutos")
        print(f"   - Ruta: {' -> '.join(ruta)}")
        print(f"   - Descripcion: Se elige esta opcion en funcion de {pref}")
        print()
        
    print("\n" + "=" * 70)
    print("CONCLUSION:")
    print("  - Auto: Mas rapido, pero mas costoso (congestion + estacionamiento)")
    print("  - Bus: Mas economico, pero mas lento (paradas + espera)")
    print("  - El sistema ayuda a elegir segun las prioridades del usuario")
    print("=" * 70)


if __name__ == '__main__':
    main()

