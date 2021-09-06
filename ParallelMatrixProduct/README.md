# Threaded Matrix Multiplication

- Vincent Alejandro Arcila
- Isabel Piedrahita Velez

# Modo de uso

## Para compilación usar el siguiente comando:

  gcc matrixMultiplication.c -pthreads -o matrixMultiplication.o

## Para correrlo se requiere un archivo de prueba con el siguiente formato, un ejemplo de este se encuentra más abajo.

FilasA ColumnasA
[A11 A12 A13 … A1n]

[A21 A22 A23 … A2n]

.
.
.

[Am1 Am2 Am3 … Amn]

FilasB ColumnasB

[B11 B12 B13 … B1m]

[B21 B22 B23 … B2m]

.
.
.

[Bk1 Bk2 Bk3 … Bkm]

--------------------

3 3 

0 1 2 

1 2 3 

2 3 4 

3 4 

0 1 2 3

1 2 3 4

2 3 4 5


Para correr el código con el caso de prueba dado en sistemas unix usar:

	./matrixMultiplication.o < archivoDePrueba
