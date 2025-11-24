"""
SISTEMA DE OPTIMIZACIÓN DE DISTRIBUCIÓN DE AGUA EN AREQUIPA
Combina Floyd-Warshall + Programación Dinámica + Divide et Impera

Problema: Durante crisis hídricas, asignar cisternas de diferentes capacidades
         a distritos afectados minimizando tiempo y costo de combustible.
"""

import sys
from typing import List, Dict, Tuple, Optional
from dataclasses import dataclass
from enum import Enum

# ============================================================================
# CLASES Y ESTRUCTURAS DE DATOS
"""
Wrapper para mantener compatibilidad: ejecuta el `main` modularizado.
El código original fue dividido en módulos para facilitar mantenimiento.
"""

from main_modular import main


if __name__ == '__main__':
    main()