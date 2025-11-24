from typing import Dict, List
from entities import TipoCisterna
from optimizador import OptimizadorCisternas


class GestorZonasArequipa:
    """
    Divide la ciudad en zonas y optimiza cada una independientemente
    Luego integra las soluciones
    """

    def __init__(self, red_vial):
        self.red_vial = red_vial
        self.zonas = {
            'Zona_Plana': [],
            'Zona_Intermedia': [],
            'Zona_Alta': []
        }

    def clasificar_distrito(self, distrito):
        if distrito.es_zona_alta:
            self.zonas['Zona_Alta'].append(distrito)
        elif distrito.prioridad == 1:
            self.zonas['Zona_Plana'].append(distrito)
        else:
            self.zonas['Zona_Intermedia'].append(distrito)

    def optimizar_por_zonas(self,
                           planta_id: int,
                           cisternas_totales: Dict[TipoCisterna, tuple],
                           presupuesto_diesel: float) -> Dict[str, List]:
        print("\n" + "="*70)
        print("DIVIDE ET IMPERA: OPTIMIZACIÓN POR ZONAS")
        print("="*70)
        resultados = {}
        total_distritos = sum(len(d) for d in self.zonas.values() if d)
        for zona, distritos in self.zonas.items():
            if not distritos:
                continue
            print(f"\n>>> Procesando {zona}:")
            print(f"    Distritos: {len(distritos)}")
            proporcion = len(distritos) / total_distritos if total_distritos > 0 else 0
            cisternas_zona = {
                TipoCisterna.GRANDE: (
                    int(cisternas_totales[TipoCisterna.GRANDE][0] * proporcion),
                    cisternas_totales[TipoCisterna.GRANDE][1]
                ),
                TipoCisterna.MEDIANA: (
                    int(cisternas_totales[TipoCisterna.MEDIANA][0] * proporcion),
                    cisternas_totales[TipoCisterna.MEDIANA][1]
                ),
                TipoCisterna.PEQUEÑA: (
                    int(cisternas_totales[TipoCisterna.PEQUEÑA][0] * proporcion),
                    cisternas_totales[TipoCisterna.PEQUEÑA][1]
                ),
            }
            diesel_zona = presupuesto_diesel * proporcion
            print(f"    Cisternas asignadas: {cisternas_zona[TipoCisterna.GRANDE][0]}G + "
                  f"{cisternas_zona[TipoCisterna.MEDIANA][0]}M + "
                  f"{cisternas_zona[TipoCisterna.PEQUEÑA][0]}P")
            print(f"    Diesel asignado: {diesel_zona:.1f}L")
            optimizador = OptimizadorCisternas(
                self.red_vial,
                planta_id,
                cisternas_zona,
                diesel_zona
            )
            asignaciones = optimizador.optimizar_asignacion(distritos)
            resultados[zona] = asignaciones
            print(f"    ✓ Zona optimizada: {len(asignaciones)} distritos atendidos")
        print("\n✓ Todas las zonas optimizadas independientemente\n")
        return resultados
