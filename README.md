# Cifrado por Rejilla de Cardano (Cardan Grille Cipher)

## Descripción General

Este programa implementa el **cifrado por rejilla de Cardano**, un método criptográfico clásico que utiliza una plantilla perforada para cifrar y descifrar mensajes. El algoritmo funciona colocando una rejilla con agujeros sobre una cuadrícula y rellenando los espacios visibles con las letras del mensaje, rotando la rejilla 90 grados en cada iteración hasta completar 4 rotaciones.

## Funcionamiento del Algoritmo

### 1. Preparación del Mensaje

#### Lectura del archivo
- **Archivo de entrada:** `MENSAJE.txt`
- **Función:** `concatenar()`
- El programa lee todo el contenido del archivo y lo almacena en una cadena de texto

#### Padding del mensaje
- **Función:** `padTextToMultipleOfFour()`
- **Propósito:** El mensaje debe tener una longitud múltiplo de 4 para funcionar correctamente con las 4 rotaciones
- **Proceso:** Si el mensaje no es múltiplo de 4, se agregan caracteres aleatorios (A-Z) al final hasta completar el múltiplo
- **Ejemplo:** Si el mensaje tiene 10 caracteres, se agregan 2 caracteres aleatorios para llegar a 12

### 2. Cálculo del Tamaño de la Rejilla

#### Determinación del tamaño
- **Función:** `calculateNextEvenSquareSize()`
- **Criterio:** Se calcula la raíz cuadrada del tamaño del mensaje (con padding)
- **Ajuste:** Si la raíz no es par, se incrementa en 1 para obtener una dimensión par
- **Razón:** Las rejillas de Cardano requieren dimensiones pares para funcionar correctamente con las rotaciones

### 3. Generación de la Rejilla

#### Cálculo de agujeros
- **Fórmula:** `holes = word.size() / 4`
- **Explicación:** Como se realizan 4 rotaciones, cada posición de la rejilla se usa exactamente 4 veces

#### Selección aleatoria de posiciones
- **Función:** `selectRandomPositionsWithReservoir()`
- **Algoritmo:** Implementa el algoritmo de muestreo por reservorio
- **Proceso:** 
  1. Se considera un cuadrante base (superior izquierdo) de la rejilla
  2. Se seleccionan aleatoriamente las posiciones donde habrá agujeros
  3. Se garantiza una distribución uniforme

#### Asignación de cuadrantes
- **Función:** `assignRandomQuadrants()`
- **Propósito:** Cada posición seleccionada se asigna aleatoriamente a uno de los 4 cuadrantes (0, 1, 2, 3)

#### Conversión a coordenadas de rejilla
- **Función:** `convertToGrilleCoordinates()`
- **Transformaciones por cuadrante:**
  - **Cuadrante 0 (superior izquierdo):** `(fila, col)`
  - **Cuadrante 1 (superior derecho):** `(col, gridSize - fila - 1)`
  - **Cuadrante 2 (inferior derecho):** `(gridSize - col - 1, fila)`
  - **Cuadrante 3 (inferior izquierdo):** `(gridSize - fila - 1, gridSize - col - 1)`

### 4. Proceso de Cifrado

#### Inicialización de la matriz
- Se crea una matriz de `gridSize x gridSize` inicializada con asteriscos ('*')

#### Llenado con rotaciones
- **Función:** `encryptWithCardanGrille()`
- **Proceso:**
  1. **Rotación 0:** Se colocan los primeros caracteres en las posiciones de la rejilla
  2. **Rotación 90°:** Se rota la rejilla y se colocan los siguientes caracteres
  3. **Rotación 180°:** Se rota nuevamente y se continúa
  4. **Rotación 270°:** Última rotación para completar el mensaje

#### Función de rotación
- **Función:** `rotateGrille90Degrees()`
- **Transformación:** Para rotar 90° en sentido horario: `(fila, col) → (col, gridSize - fila - 1)`

#### Ordenamiento de posiciones
- **Función:** `fillMatrixWithRotatedGrille()`
- Las posiciones se ordenan para llenar la matriz de manera sistemática (de izquierda a derecha, de arriba hacia abajo)

#### Llenado de espacios vacíos
- **Función:** `fillRemainingWithRandomLetters()`
- Todos los espacios que quedaron con '*' se llenan con letras aleatorias (A-Z)
- **Propósito:** Ocultar el patrón real del mensaje entre caracteres de relleno

### 5. Guardado del Resultado Cifrado

El archivo `MENSAJE-CIFRADO.txt` contiene:

```
[tamaño_original]
[tamaño_de_rejilla]
[patrón_de_rejilla: '0' = agujero, '#' = bloqueado]
[matriz_cifrada: todas las letras incluyendo relleno aleatorio]
```

**Ejemplo de estructura:**
```
10
4
0##0
#00#
#00#
0##0
XMYB
QFGH
IJKL
AZPW
```

### 6. Proceso de Descifrado

#### Lectura del archivo cifrado
- **Funciones:** `readGrilleFromFile()` y `readEncryptedMatrix()`
- Se extrae la rejilla original (posiciones marcadas con '0')
- Se carga la matriz cifrada completa

#### Extracción del mensaje
- **Función:** `decryptWithCardanGrille()`
- **Proceso:**
  1. Se aplica la rejilla en su posición inicial
  2. Se extraen los caracteres visibles a través de los agujeros
  3. Se rota la rejilla 90° y se repite
  4. Se continúa hasta completar las 4 rotaciones
  5. Se concatenan todos los caracteres extraídos

#### Eliminación del padding
- Se trunca el mensaje desencriptado al tamaño original usando `substr(0, originalSize)`
- Esto elimina los caracteres aleatorios que se agregaron durante el padding

#### Guardado del resultado
- **Archivo de salida:** `MENSAJE_DESCIFRADO.txt`
- Contiene únicamente el mensaje original sin padding ni caracteres de relleno

## Archivos Involucrados

### Entrada
- **`MENSAJE.txt`:** Contiene el mensaje original a cifrar

### Salida
- **`MENSAJE-CIFRADO.txt`:** Contiene la rejilla y la matriz cifrada
- **`MENSAJE_DESCIFRADO.txt`:** Contiene el mensaje descifrado original

## Seguridad del Algoritmo

### Fortalezas
- **Ocultación del patrón:** Los caracteres aleatorios ocultan la estructura real del mensaje
- **Rejilla aleatoria:** Cada ejecución genera una rejilla diferente
- **Distribución uniforme:** El algoritmo de reservorio garantiza una selección equitativa de posiciones

### Consideraciones
- **Clave secreta:** La rejilla funciona como la clave del cifrado
- **Tamaño variable:** Se adapta automáticamente al tamaño del mensaje
- **Resistencia:** La seguridad depende de mantener secreta la plantilla de la rejilla

## Ejemplo de Funcionamiento

Para el mensaje `"TESTABCDE!"` (10 caracteres):

1. **Padding:** Se agregan 2 caracteres aleatorios → `"TESTABCDE!XY"` (12 caracteres)
2. **Rejilla:** Se calcula una rejilla 4x4 (siguiente par mayor que √12 ≈ 3.46)
3. **Agujeros:** Se necesitan 12/4 = 3 agujeros
4. **Cifrado:** Se realizan 4 rotaciones colocando 3 caracteres en cada una
5. **Resultado:** Matriz 4x4 con mensaje oculto entre caracteres aleatorios

## Compilación y Ejecución

```bash
g++ -o cardan_grille main.cpp
./cardan_grille
```

**Requisitos:**
- Archivo `MENSAJE.txt` en el mismo directorio
- Compilador C++ compatible con C++11 o superior

## Notas Técnicas

- **Generador aleatorio:** Utiliza `std::random_device` y `std::mt19937` para mejor calidad aleatoria
- **Algoritmo de reservorio:** Implementación eficiente para selección aleatoria uniforme
- **Manejo de archivos:** Lectura y escritura robusta con validación implícita
- **Rotaciones matemáticas:** Transformaciones geométricas precisas para las rotaciones de 90°