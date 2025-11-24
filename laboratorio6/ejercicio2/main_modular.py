from entities import TipoCisterna, Cisterna, Distrito
from red_vial import RedVialArequipa
from gestor_zonas import GestorZonasArequipa


def main():
    print("\n" + "="*70)
    print(" "*15 + "SISTEMA DE DISTRIBUCIÓN DE AGUA")
    print(" "*10 + "Crisis Hídrica en Arequipa - SEDAPAR")
    print("="*70)

    # 1. CREAR RED VIAL
    print("\n[1] CONFIGURANDO RED VIAL DE AREQUIPA")
    print("-" * 70)
    red = RedVialArequipa(10)
    red.agregar_nodo(0, "Planta_Tomilla")
    nombres_distritos = [
        "Cercado", "Hunter", "JLByR", "Yanahuara",
        "Cayma", "Alto_Selva", "Paucarpata", "Mariano_Melgar", "Cerro_Colorado"
    ]
    for i, nombre in enumerate(nombres_distritos, start=1):
        red.agregar_nodo(i, nombre)
    print(f"✓ {red.n} nodos creados (1 planta + 9 distritos)")
    calles = [
        (0, 1, 4.2), (0, 2, 5.8), (0, 3, 6.5), (0, 4, 3.8), (0, 5, 7.2),
        (1, 2, 2.5), (1, 3, 3.1), (1, 4, 2.8), (2, 3, 1.9), (4, 5, 4.5),
        (5, 6, 5.2), (3, 7, 4.8), (7, 8, 3.6), (8, 9, 4.1), (6, 9, 6.3)
    ]
    for origen, destino, distancia in calles:
        red.agregar_calle(origen, destino, distancia)
    print(f"✓ {len(calles)} calles agregadas (bidireccionales)")

    # 2. FLOYD-WARSHALL
    red.calcular_floyd_warshall()
    red.mostrar_matriz_distancias()

    # 3. CISTERNAS
    print("\n[2] CONFIGURANDO FLOTA DE CISTERNAS")
    print("-" * 70)
    cisterna_grande = Cisterna(
        tipo=TipoCisterna.GRANDE,
        capacidad_litros=10000,
        velocidad_kmh=40,
        consumo_diesel_por_100km=25.0,
        puede_subir_pendiente=False
    )
    cisterna_mediana = Cisterna(
        tipo=TipoCisterna.MEDIANA,
        capacidad_litros=5000,
        velocidad_kmh=55,
        consumo_diesel_por_100km=18.0,
        puede_subir_pendiente=True
    )
    cisterna_pequeña = Cisterna(
        tipo=TipoCisterna.PEQUEÑA,
        capacidad_litros=2000,
        velocidad_kmh=65,
        consumo_diesel_por_100km=12.0,
        puede_subir_pendiente=True
    )
    cisternas_disponibles = {
        TipoCisterna.GRANDE: (20, cisterna_grande),
        TipoCisterna.MEDIANA: (30, cisterna_mediana),
        TipoCisterna.PEQUEÑA: (17, cisterna_pequeña),
    }
    print("Flota disponible:")
    for tipo, (cantidad, cisterna) in cisternas_disponibles.items():
        print(f"  {cantidad:2d}x {cisterna}")
    presupuesto_diesel = 5000.0
    print(f"\nPresupuesto de diesel: {presupuesto_diesel:,.1f} litros")

    # 4. DISTRITOS
    print("\n[3] CONFIGURANDO DISTRITOS AFECTADOS")
    print("-" * 70)
    distritos = [
        Distrito(1, "Cercado", 58000, 180000, False, 1),
        Distrito(2, "Hunter", 45000, 140000, False, 2),
        Distrito(3, "JLByR", 75000, 230000, False, 2),
        Distrito(4, "Yanahuara", 28000, 85000, False, 2),
        Distrito(5, "Cayma", 82000, 250000, False, 2),
        Distrito(6, "Alto_Selva", 95000, 290000, True, 3),
        Distrito(7, "Paucarpata", 125000, 380000, False, 2),
        Distrito(8, "Mariano_Melgar", 68000, 210000, True, 3),
        Distrito(9, "Cerro_Colorado", 150000, 460000, True, 3),
    ]
    print(f"Total: {len(distritos)} distritos afectados\n")
    for distrito in distritos:
        print(f"  {distrito}")

    # 5. DIVIDE ET IMPERA
    gestor_zonas = GestorZonasArequipa(red)
    for distrito in distritos:
        gestor_zonas.clasificar_distrito(distrito)
    resultados_por_zona = gestor_zonas.optimizar_por_zonas(
        planta_id=0,
        cisternas_totales=cisternas_disponibles,
        presupuesto_diesel=presupuesto_diesel
    )

    # 6. MOSTRAR RESULTADOS
    print("\n" + "="*70)
    print("RESULTADOS FINALES - ASIGNACIÓN ÓPTIMA")
    print("="*70)
    todas_asignaciones = []
    for zona, asignaciones in resultados_por_zona.items():
        todas_asignaciones.extend(asignaciones)
    todas_asignaciones.sort(key=lambda a: a.distrito.prioridad)
    poblacion_total_atendida = 0
    litros_totales = 0
    diesel_total = 0
    tiempo_maximo = 0
    cisternas_usadas = {'grandes': 0, 'medianas': 0, 'pequeñas': 0}
    for asig in todas_asignaciones:
        print(asig)
        poblacion_atendida = asig.distrito.poblacion * (asig.litros_entregados / asig.distrito.demanda_litros)
        poblacion_total_atendida += poblacion_atendida
        litros_totales += asig.litros_entregados
        diesel_total += asig.diesel_consumido
        tiempo_maximo = max(tiempo_maximo, asig.tiempo_total_min)
        cisternas_usadas['grandes'] += asig.cisternas_grandes
        cisternas_usadas['medianas'] += asig.cisternas_medianas
        cisternas_usadas['pequeñas'] += asig.cisternas_pequeñas

    print("\n" + "="*70)
    print("RESUMEN GLOBAL")
    print("="*70)
    print(f"Población total atendida: {poblacion_total_atendida:,.0f} habitantes")
    print(f"Litros totales distribuidos: {litros_totales:,} L")
    print(f"Diesel consumido: {diesel_total:.2f}L / {presupuesto_diesel:.1f}L "
          f"({(diesel_total/presupuesto_diesel)*100:.1f}% )")
    print(f"Tiempo máximo de operación: {tiempo_maximo:.1f} minutos")
    print(f"\nCisternas utilizadas:")
    print(f"  Grandes: {cisternas_usadas['grandes']}/{cisternas_disponibles[TipoCisterna.GRANDE][0]}")
    print(f"  Medianas: {cisternas_usadas['medianas']}/{cisternas_disponibles[TipoCisterna.MEDIANA][0]}")
    print(f"  Pequeñas: {cisternas_usadas['pequeñas']}/{cisternas_disponibles[TipoCisterna.PEQUEÑA][0]}")
    print("\n" + "="*70)
    V = red.n
    D = len(distritos)
    G, M, P = cisternas_disponibles[TipoCisterna.GRANDE][0], cisternas_disponibles[TipoCisterna.MEDIANA][0], cisternas_disponibles[TipoCisterna.PEQUEÑA][0]
    print("COMPLEJIDAD COMPUTACIONAL")
    print("="*70)
    print(f"Floyd-Warshall: O(V³) = O({V}³) = {V**3:,} operaciones")
    print(f"Programación Dinámica: O(D × G × M × P) ≈ O({D} × {G} × {M} × {P}) = {D*G*M*P:,} estados")
    print(f"Total: O(V³ + D×G×M×P) = {V**3 + D*G*M*P:,} operaciones")
    print("="*70 + "\n")


if __name__ == '__main__':
    main()
