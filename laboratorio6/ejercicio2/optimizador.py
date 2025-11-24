from typing import List, Tuple, Dict, Optional
import time
from entities import TipoCisterna, Asignacion, Distrito


class OptimizadorCisternas:
    """
    Optimiza la asignación de cisternas a distritos usando Programación Dinámica
    Incluye: heurística inicial (golosa), límite de tiempo (timeout) y contador de progreso.
    """

    def __init__(self,
                 red_vial,
                 planta_id: int,
                 cisternas_disponibles: Dict[TipoCisterna, Tuple[int, object]],
                 presupuesto_diesel: float):
        self.red_vial = red_vial
        self.planta_id = planta_id
        self.cisternas_disponibles = cisternas_disponibles
        self.presupuesto_diesel = presupuesto_diesel
        self.memo = {}

    def calcular_tiempo_viaje(self,
                              distrito_id: int,
                              tipo_cisterna: TipoCisterna) -> float:
        distancia_km = self.red_vial.obtener_distancia(self.planta_id, distrito_id)
        cisterna = self.cisternas_disponibles[tipo_cisterna][1]
        tiempo_min = (distancia_km * 2) / cisterna.velocidad_kmh * 60
        tiempo_min += 20
        return tiempo_min

    def calcular_diesel_consumido(self,
                                  distrito_id: int,
                                  tipo_cisterna: TipoCisterna) -> float:
        distancia_km = self.red_vial.obtener_distancia(self.planta_id, distrito_id)
        cisterna = self.cisternas_disponibles[tipo_cisterna][1]
        diesel = (distancia_km * 2) * (cisterna.consumo_diesel_por_100km / 100)
        return diesel

    def _solucion_golosa_inicial(self, distritos: List[Distrito]):
        """Genera una solución inicial rápida (golosa) por distrito.
        Devuelve (asignaciones, recursos_restantes_diesel, cisternas_restantes)
        """
        asignaciones = []
        diesel_restante = self.presupuesto_diesel
        cisternas_restantes = {t: self.cisternas_disponibles[t][0] for t in self.cisternas_disponibles}

        for distrito in sorted(distritos, key=lambda d: d.prioridad):
            mejor = None
            mejor_valor = -1
            # probar combinaciones pequeñas razonables
            for g in range(0, min(3, cisternas_restantes.get(TipoCisterna.GRANDE, 0)) + 1):
                for m in range(0, min(4, cisternas_restantes.get(TipoCisterna.MEDIANA, 0)) + 1):
                    for p in range(0, min(5, cisternas_restantes.get(TipoCisterna.PEQUEÑA, 0)) + 1):
                        if g + m + p == 0:
                            continue
                        if distrito.es_zona_alta and g > 0:
                            continue
                        litros = (
                            g * self.cisternas_disponibles[TipoCisterna.GRANDE][1].capacidad_litros +
                            m * self.cisternas_disponibles[TipoCisterna.MEDIANA][1].capacidad_litros +
                            p * self.cisternas_disponibles[TipoCisterna.PEQUEÑA][1].capacidad_litros
                        )
                        diesel = (
                            g * self.calcular_diesel_consumido(distrito.id, TipoCisterna.GRANDE) +
                            m * self.calcular_diesel_consumido(distrito.id, TipoCisterna.MEDIANA) +
                            p * self.calcular_diesel_consumido(distrito.id, TipoCisterna.PEQUEÑA)
                        )
                        if diesel > diesel_restante:
                            continue
                        cobertura = min(1.0, litros / distrito.demanda_litros)
                        valor = distrito.poblacion * cobertura * (4 - distrito.prioridad)
                        if valor > mejor_valor:
                            mejor_valor = valor
                            mejor = (g, m, p, litros, diesel)

            if mejor:
                g, m, p, litros, diesel = mejor
                asignaciones.append(Asignacion(distrito, g, m, p, 0.0, litros, diesel))
                diesel_restante -= diesel
                cisternas_restantes[TipoCisterna.GRANDE] -= g
                cisternas_restantes[TipoCisterna.MEDIANA] -= m
                cisternas_restantes[TipoCisterna.PEQUEÑA] -= p

        return asignaciones, diesel_restante, cisternas_restantes

    def optimizar_asignacion(self, distritos: List[Distrito], tiempo_max_segundos: Optional[float] = 10.0,
                             progreso_interval: int = 5000) -> List[Asignacion]:
        """
        Ejecuta la optimización con timeout y heurística inicial.
        - tiempo_max_segundos: si se alcanza, se devuelve la mejor solución encontrada
        - progreso_interval: cada N estados imprime progreso
        """
        print("\n" + "="*70)
        print("EJECUTANDO PROGRAMACIÓN DINÁMICA (con timeout y heurística)")
        print("="*70)
        print("Optimizando asignación de cisternas a distritos...\n")

        start_time = time.time()
        estados_explorados = 0
        mejor_global_valor = -1.0
        mejor_global_asignacion = []

        distritos_ordenados = sorted(distritos, key=lambda d: d.prioridad)

        # Recursos totales (para límites en DP)
        max_grandes = self.cisternas_disponibles[TipoCisterna.GRANDE][0]
        max_medianas = self.cisternas_disponibles[TipoCisterna.MEDIANA][0]
        max_pequeñas = self.cisternas_disponibles[TipoCisterna.PEQUEÑA][0]

        # Solución golosa inicial
        sol_golosa, diesel_rest, cisternas_rest = self._solucion_golosa_inicial(distritos)
        if sol_golosa:
            valor_golosa = sum(a.distrito.poblacion * min(1.0, a.litros_entregados / a.distrito.demanda_litros) * (4 - a.distrito.prioridad) for a in sol_golosa)
            mejor_global_valor = valor_golosa
            mejor_global_asignacion = sol_golosa
            print(f"Solución golosa inicial: valor={valor_golosa:,.0f}, asignaciones={len(sol_golosa)}")

        # estado para memo cache; lo mantenemos local
        self.memo = {}

        # variable para poder devolver la mejor encontrada si hay timeout
        mejor_en_busqueda_valor = mejor_global_valor
        mejor_en_busqueda_asignacion = mejor_global_asignacion

        def dp(distrito_idx: int,
               grandes_usadas: int,
               medianas_usadas: int,
               pequeñas_usadas: int,
               diesel_usado: float):
            nonlocal estados_explorados, mejor_en_busqueda_valor, mejor_en_busqueda_asignacion

            # timeout check
            if tiempo_max_segundos is not None and (time.time() - start_time) > tiempo_max_segundos:
                raise TimeoutError()

            estados_explorados += 1
            if estados_explorados % progreso_interval == 0:
                print(f"  Estados explorados: {estados_explorados:,} (tiempo {time.time()-start_time:.1f}s)")

            if distrito_idx >= len(distritos_ordenados):
                return 0.0, []

            estado = (distrito_idx, grandes_usadas, medianas_usadas,
                      pequeñas_usadas, int(diesel_usado * 10))
            if estado in self.memo:
                return self.memo[estado]

            distrito = distritos_ordenados[distrito_idx]
            mejor_poblacion = 0.0
            mejor_asignacion = []

            for g in range(0, min(4, max_grandes - grandes_usadas + 1)):
                for m in range(0, min(6, max_medianas - medianas_usadas + 1)):
                    for p in range(0, min(8, max_pequeñas - pequeñas_usadas + 1)):
                        if distrito.es_zona_alta and g > 0:
                            continue
                        if g + m + p == 0:
                            pob_resto, asig_resto = dp(distrito_idx + 1,
                                                       grandes_usadas,
                                                       medianas_usadas,
                                                       pequeñas_usadas,
                                                       diesel_usado)
                            if pob_resto > mejor_poblacion:
                                mejor_poblacion = pob_resto
                                mejor_asignacion = asig_resto
                            continue

                        litros_entregados = (
                            g * self.cisternas_disponibles[TipoCisterna.GRANDE][1].capacidad_litros +
                            m * self.cisternas_disponibles[TipoCisterna.MEDIANA][1].capacidad_litros +
                            p * self.cisternas_disponibles[TipoCisterna.PEQUEÑA][1].capacidad_litros
                        )

                        tiempos = []
                        if g > 0:
                            tiempos.append(self.calcular_tiempo_viaje(distrito.id, TipoCisterna.GRANDE))
                        if m > 0:
                            tiempos.append(self.calcular_tiempo_viaje(distrito.id, TipoCisterna.MEDIANA))
                        if p > 0:
                            tiempos.append(self.calcular_tiempo_viaje(distrito.id, TipoCisterna.PEQUEÑA))

                        tiempo_total = max(tiempos) if tiempos else 0

                        diesel_consumido = (
                            g * self.calcular_diesel_consumido(distrito.id, TipoCisterna.GRANDE) +
                            m * self.calcular_diesel_consumido(distrito.id, TipoCisterna.MEDIANA) +
                            p * self.calcular_diesel_consumido(distrito.id, TipoCisterna.PEQUEÑA)
                        )

                        if diesel_usado + diesel_consumido > self.presupuesto_diesel:
                            continue

                        cobertura = min(1.0, litros_entregados / distrito.demanda_litros)
                        poblacion_atendida = distrito.poblacion * cobertura * (4 - distrito.prioridad)

                        pob_resto, asig_resto = dp(distrito_idx + 1,
                                                   grandes_usadas + g,
                                                   medianas_usadas + m,
                                                   pequeñas_usadas + p,
                                                   diesel_usado + diesel_consumido)

                        poblacion_total = poblacion_atendida + pob_resto

                        if poblacion_total > mejor_poblacion:
                            mejor_poblacion = poblacion_total
                            asignacion_actual = Asignacion(
                                distrito=distrito,
                                cisternas_grandes=g,
                                cisternas_medianas=m,
                                cisternas_pequeñas=p,
                                tiempo_total_min=tiempo_total,
                                litros_entregados=litros_entregados,
                                diesel_consumido=diesel_consumido
                            )
                            mejor_asignacion = [asignacion_actual] + asig_resto

            self.memo[estado] = (mejor_poblacion, mejor_asignacion)

            # actualizar mejor global encontrado durante la búsqueda
            if mejor_poblacion > mejor_en_busqueda_valor:
                mejor_en_busqueda_valor = mejor_poblacion
                mejor_en_busqueda_asignacion = mejor_asignacion

            return mejor_poblacion, mejor_asignacion

        # Ejecutar DP con manejo de timeout
        try:
            poblacion_total, asignaciones = dp(0, 0, 0, 0, 0.0)
            mejor_final_valor = poblacion_total
            mejor_final_asignacion = asignaciones
        except TimeoutError:
            print("\n[!] Timeout alcanzado durante la búsqueda. Devolviendo mejor solución encontrada hasta ahora.")
            mejor_final_valor = mejor_en_busqueda_valor
            mejor_final_asignacion = mejor_en_busqueda_asignacion

        # Si la búsqueda no mejoró la golosa, mantener la golosa
        if mejor_global_valor > mejor_final_valor:
            print(f"Mejor solución final: valor golosa={mejor_global_valor:,.0f} > valor DP={mejor_final_valor:,.0f} (se mantiene golosa)")
            resultado = mejor_global_asignacion
        else:
            resultado = mejor_final_asignacion

        print(f"✓ Optimización completada")
        print(f"✓ Estados explorados: {estados_explorados:,}")
        print(f"✓ Mejor valor (ponderado): {max(mejor_global_valor, mejor_final_valor):,.0f}\n")
        return resultado
