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

### 6. Proceso de Descifrado (Análisis Detallado)

El descifrado es el proceso **inverso exacto** del cifrado. Es crucial entender que la rejilla funciona como una **llave maestra** que revela las posiciones exactas donde se colocó el mensaje original.

#### 6.1. Lectura y Reconstrucción del Archivo Cifrado

##### Función `readGrilleFromFile()`
```cpp
vector<pair<int,int>> readGrilleFromFile(const string& filename, int& sizeOriginal, int& gridSize)
```

**Proceso paso a paso:**
1. **Lee los metadatos:** Las primeras dos líneas del archivo contienen:
   - `sizeOriginal`: El tamaño real del mensaje antes del padding
   - `gridSize`: Las dimensiones de la rejilla cuadrada

2. **Reconstruye la rejilla:** Examina cada carácter de la matriz de rejilla:
   - **'0'** = Posición con agujero (donde se colocaron letras del mensaje)
   - **'#'** = Posición bloqueada (donde se colocaron letras aleatorias)

3. **Almacena coordenadas:** Cada vez que encuentra un '0', guarda las coordenadas `(fila, columna)` en un vector

**Ejemplo de lectura:**
```
Archivo: MENSAJE-CIFRADO.txt
10        ← sizeOriginal = 10
4         ← gridSize = 4
0##0      ← Fila 0: agujeros en (0,0) y (0,3)
#00#      ← Fila 1: agujeros en (1,1) y (1,2)
#00#      ← Fila 2: agujeros en (2,1) y (2,2)
0##0      ← Fila 3: agujeros en (3,0) y (3,3)
```

**Resultado:** `rejilla = [(0,0), (0,3), (1,1), (1,2), (2,1), (2,2), (3,0), (3,3)]`

##### Función `readEncryptedMatrix()`
```cpp
vector<vector<char>> readEncryptedMatrix(const string& filename, int gridSize)
```

**Proceso:**
1. **Salta los metadatos:** Lee y descarta las primeras líneas (tamaños y rejilla)
2. **Carga la matriz cifrada:** Lee las siguientes `gridSize` líneas que contienen la matriz con:
   - Caracteres del mensaje original
   - Caracteres de relleno aleatorios

**Ejemplo:**
```
XMYB      ← Fila 0 de la matriz cifrada
QFGH      ← Fila 1 de la matriz cifrada
IJKL      ← Fila 2 de la matriz cifrada
AZPW      ← Fila 3 de la matriz cifrada
```

#### 6.2. Proceso de Extracción del Mensaje

##### Función `decryptWithCardanGrille()`
```cpp
string decryptWithCardanGrille(const vector<vector<char>>& matrix, vector<pair<int,int>> grille, int gridSize)
```

Esta es la función **más crítica** del descifrado. Aquí está el análisis detallado:

**Bucle principal:** Se ejecuta exactamente 4 veces (una por cada rotación)

##### **ROTACIÓN 0 (Posición Original)**
1. **Ordenamiento de la rejilla:**
   ```cpp
   vector<pair<int,int>> sortedGrille = grille;
   sort(sortedGrille.begin(), sortedGrille.end());
   ```
   - **Propósito:** Garantiza que se lean los caracteres en orden correcto (izquierda→derecha, arriba→abajo)
   - **Ejemplo:** `[(0,0), (0,3), (1,1), (1,2), (2,1), (2,2), (3,0), (3,3)]`

2. **Extracción de caracteres:**
   ```cpp
   for(const auto& pos : sortedGrille) {
       decryptedText += matrix[pos.first][pos.second];
   }
   ```
   - Lee cada posición de la matriz cifrada donde hay un agujero
   - **Ejemplo:** Si la rejilla apunta a `(0,0), (0,3), (1,1)...` extrae `X, B, F...`

##### **ROTACIÓN 1 (90° en sentido horario)**
3. **Rotación de la rejilla:**
   ```cpp
   rotateGrille90Degrees(grille, gridSize);
   ```
   - **Transformación matemática:** `(fila, col) → (col, gridSize - fila - 1)`
   - **Ejemplo:** `(0,0) → (0,3)`, `(0,3) → (3,3)`, `(1,1) → (1,2)`

4. **Nueva extracción:** Se repite el proceso con las nuevas posiciones rotadas

##### **ROTACIONES 2 y 3 (180° y 270°)**
5. **Continúa el proceso:** Se aplican las rotaciones restantes, extrayendo más caracteres

#### 6.3. Ejemplo Completo de Descifrado

Supongamos que tenemos esta matriz cifrada:
```
T E S X
R A N Y
D B C Z
Q M W F
```

Y esta rejilla (posiciones con '0'):
```
0 # # 0
# 0 0 #
# 0 0 #
0 # # 0
```

**Proceso detallado:**

##### **ROTACIÓN 0:**
- **Posiciones de agujeros:** `(0,0), (0,3), (1,1), (1,2), (2,1), (2,2), (3,0), (3,3)`
- **Caracteres extraídos:** `T, X, A, N, B, C, Q, F`
- **Mensaje parcial:** `"TXANBCQF"`

##### **ROTACIÓN 1 (90°):**
- **Posiciones rotadas:** `(0,3) → (3,3)`, `(0,0) → (0,3)`, etc.
- **Nuevas posiciones:** `(0,3), (3,3), (1,2), (2,1), (1,1), (2,2), (3,0), (0,0)`
- **Caracteres extraídos:** `X, F, N, B, A, C, Q, T`
- **Mensaje parcial:** `"TXANBCQF" + "XFNBACQT"`

##### **ROTACIONES 2 y 3:**
- Se continúa el proceso hasta completar las 4 rotaciones
- **Resultado:** Cadena completa con todos los caracteres del mensaje original + padding

#### 6.4. Eliminación del Padding y Finalización

##### Truncamiento al tamaño original
```cpp
decryptedMessage = decryptedMessage.substr(0, originalSize);
```

**¿Por qué funciona esto?**
- Durante el cifrado, el mensaje se rellenó con caracteres aleatorios
- El `originalSize` se guardó antes del padding
- Al truncar, se eliminan automáticamente los caracteres de relleno
- **Ejemplo:** Si `originalSize = 10` y el mensaje descifrado es `"TESTABCDE!XY"`, se trunca a `"TESTABCDE!"`

##### Guardado final
```cpp
ofstream outDecrypted("MENSAJE_DESCIFRADO.txt");
outDecrypted << decryptedMessage;
```

#### 6.5. ¿Por Qué Funciona el Descifrado?

**Principio fundamental:** La rejilla de Cardano garantiza que:

1. **Cada posición se usa exactamente una vez por rotación**
2. **No hay solapamientos entre rotaciones**
3. **El orden de llenado se preserva mediante el ordenamiento**
4. **Las rotaciones matemáticas son exactamente inversas al cifrado**

**Matemáticamente:**
- Si durante el cifrado se colocó el carácter `C` en la posición `(i,j)` durante la rotación `R`
- Durante el descifrado, la rotación `R` volverá a exponer exactamente la posición `(i,j)`
- El ordenamiento garantiza que `C` se extraiga en la posición correcta de la secuencia

**Resultado:** El mensaje se reconstruye **carácter por carácter** en el orden exacto en que se cifró.

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