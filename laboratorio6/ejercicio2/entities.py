from dataclasses import dataclass
from enum import Enum
from typing import List


class TipoCisterna(Enum):
    """Tipos de cisternas disponibles"""
    GRANDE = "Grande"
    MEDIANA = "Mediana"
    PEQUEÑA = "Pequeña"


@dataclass
class Cisterna:
    """Representa una cisterna con sus características"""
    tipo: TipoCisterna
    capacidad_litros: int
    velocidad_kmh: float
    consumo_diesel_por_100km: float
    puede_subir_pendiente: bool

    def __str__(self):
        return (f"{self.tipo.value}: {self.capacidad_litros}L, "
                f"{self.velocidad_kmh}km/h, {self.consumo_diesel_por_100km}L/100km")


@dataclass
class Distrito:
    """Representa un distrito con su demanda de agua"""
    id: int
    nombre: str
    poblacion: int
    demanda_litros: int
    es_zona_alta: bool
    prioridad: int

    def __str__(self):
        tipo_zona = "Alta" if self.es_zona_alta else "Plana"
        return (f"{self.nombre} ({tipo_zona}): {self.poblacion:,} hab, "
                f"Demanda: {self.demanda_litros:,}L, Prioridad: {self.prioridad}")


@dataclass
class Asignacion:
    """Resultado de asignación de cisternas a un distrito"""
    distrito: Distrito
    cisternas_grandes: int
    cisternas_medianas: int
    cisternas_pequeñas: int
    tiempo_total_min: float
    litros_entregados: int
    diesel_consumido: float

    def poblacion_atendida_pct(self) -> float:
        return min(100.0, (self.litros_entregados / self.distrito.demanda_litros) * 100)

    def __str__(self):
        return (f"\n  Distrito: {self.distrito.nombre}\n"
                f"  Cisternas: {self.cisternas_grandes}G + {self.cisternas_medianas}M + "
                f"{self.cisternas_pequeñas}P\n"
                f"  Litros entregados: {self.litros_entregados:,}L "
                f"({self.poblacion_atendida_pct():.1f}% de demanda)\n"
                f"  Tiempo: {self.tiempo_total_min:.1f} min\n"
                f"  Diesel: {self.diesel_consumido:.2f}L")
