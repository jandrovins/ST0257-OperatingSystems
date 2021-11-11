#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

// Función que calcula el número de pagefaults con FIFO
int FIFOPageFaultNum(int pages[], int n, int capacity)
{
    // Set no ordenado para guardar las páginas que se encuentran cargadas a
    // frames, para que sea más rápido buscar
    unordered_set<int> frames;

    // Usamos una fila para guardar el orden de ingreso de las páginas y saber
    // quien fue first in para que pueda ser first out
    queue<int> indexesFirstIn;

    // Iteramos la cadena de referencia
    int pageFaults = 0;
    for (int i = 0; i < n; i++) {
        // Si hay frames disponibles solo ponemos la página en un frame
        if (frames.size() < capacity) {
            if (frames.find(pages[i]) == frames.end()) // Verificamos que la página no esté cargada
            {
                frames.insert(pages[i]); // Insertar a frame
                pageFaults++;
                indexesFirstIn.push(pages[i]); // Registrar ingreso en la fila
            }
        }

        // Si el set está lleno, necesitamos reemplazar una página por la nueva
        else {
            if (frames.find(pages[i]) == frames.end()) {
                // Revisamos en la fila quien fue el primero en entrar y lo removemos de
                // la fila y del set que representa los frames
                int val = indexesFirstIn.front();
                indexesFirstIn.pop();
                frames.erase(val);
                frames.insert(pages[i]); // Insertar a frame
                indexesFirstIn.push(
                    pages[i]); // Registrar en la fila el ingreso de la nueva página
                pageFaults++;
            }
        }
    }

    return pageFaults;
}

// Función para calcular número de page faults usando LRU
int LRUPageFaultNum(int pages[], int n, int capacity)
{
    // Set no ordenado para guardar las páginas que se encuentran cargadas a
    // frames, para que sea más rápido buscar
    unordered_set<int> frames;

    // Mapa para registrar cual de las páginas cargadas fue la que se usó hace más
    // tiempo
    unordered_map<int, int> indexesLeastRecentlyUsed;

    int pageFaults = 0;
    // Iteramos la cadena de referencia
    for (int i = 0; i < n; i++) {
        // Si hay espacio la ponemos, si no hay espacio reeplazamos
        if (frames.size() < capacity) {
            if (frames.find(pages[i]) == frames.end()) {
                frames.insert(pages[i]);
                pageFaults++;
            }
            indexesLeastRecentlyUsed[pages[i]] = i;
        } else {
            if (frames.find(pages[i]) == frames.end()) {
                // Iteramos el mapa de último uso para ver que valor de los frames
                // debería salir
                int lru = INT_MAX, val;
                for (auto it = frames.begin(); it != frames.end(); it++) {
                    if (indexesLeastRecentlyUsed[*it] < lru) {
                        lru = indexesLeastRecentlyUsed[*it];
                        val = *it;
                    }
                }
                frames.erase(val);       // Eliminamos el valor de los frames
                frames.insert(pages[i]); // Insertamos el nuyevo valor en su lugar
                pageFaults++;
            }
            indexesLeastRecentlyUsed[pages[i]] = i; // Registramos este uso en el mapa de último uso
        }
    }
    return pageFaults;
}

// Función auxiliar del algoritmo óptimo que predice el futuro para ver que página no se va a usar en el mayor tiempo
int predict(int pages[], vector<int>& frames, int n, int index)
{
    int sacar = -1;
    int farthest = index; // Guarda el índice de uso más lejano
    for (int i = 0; i < frames.size(); i++) {
        int j;
        for (j = index; j < n; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    sacar = i;
                }
                break;
            }
        }
        // Retornamos las páginas que no se vuelven a usar nunca
        if (j == n)
            return i;
    }
    // Si todas las páginas cargadas no vuelven a usarse devolvemos 0, de lo contrario retornamos sacar
    return (sacar == -1) ? 0 : sacar;
}

// Función para buscar si una página se encuentra cargada a un frame
// Necesaria para OPT ya que no se implementó un set sino un vector
bool search(int key, vector<int>& frames)
{
    for (int i = 0; i < frames.size(); i++)
        if (frames[i] == key)
            return true;
    return false;
}

int OPTPageFaultNum(int pages[], int n, int capacity)
{
    vector<int> frames;
    int pageFaults = 0;
    for (int i = 0; i < n; i++) {
        // if (search(page[i], fr)) {
        // hit++;
        // continue;
        //}
        // If a page not found in a frame : MISS
        // check if there is space available in frames.
        if (!search(pages[i], frames)) {
            if (frames.size() < capacity) {
                frames.push_back(pages[i]);
                pageFaults++;
            }
            // Find the page to be replaced.
            else {
                int j = predict(pages, frames, n, i + 1);
                frames[j] = pages[i];
                pageFaults++;
            }
        }
    }
    return pageFaults;
}
