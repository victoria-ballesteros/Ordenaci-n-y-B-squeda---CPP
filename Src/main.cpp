#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <cctype>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>

#include "auxiliares.h"
#include "Controller/Controller.h"

// Instancias externas
auxiliares auxiliar;
registros registro;

void ordenamientoQuicksort();
int dividirQuicksort(std::vector<Producto> &producto, int izquierda, int derecha);
void quicksort(std::vector<Producto> &producto, int izquierda, int derecha);

int main()
{
    int opcionMenuPrincipal = 0, opcionMenuSecundario = 0;

    std::cout << "1. Mostrar Registros Binarios\n";
    std::cout << "2. Ordenar Registros Binarios\n";
    opcionMenuPrincipal = auxiliar.validarInt(2);

    if (opcionMenuPrincipal == 1)
    {
        system("cls");
        std::cout << "1. Imprimir Registros: Cliente.bin\n";
        std::cout << "2. Imprimir Registros: Producto.bin\n";
        opcionMenuSecundario = auxiliar.validarInt(2);
        if (opcionMenuSecundario == 1)
        {
            system("cls");
            registro.listarClientes();
        }
        else if (opcionMenuSecundario == 2)
        {
            system("cls");
            registro.listarProductos();
        }
    }
    else if (opcionMenuPrincipal == 2)
    {
        system("cls");
        std::cout << "1. Ordenar Alfabeticamente: Producto.bin\n";
        opcionMenuSecundario = auxiliar.validarInt(1);

        if (opcionMenuSecundario == 1)
        {
            ordenamientoQuicksort();
        }
    }
}

void ordenamientoQuicksort()
{
    int i = 1;
    // Vector Producto (Memoria Interna)
    std::ifstream file("../Database/Producto.bin", std::ios::binary);
    std::vector<Producto> productos;

    if (!file.is_open())
    {
        std::cout << "No se pudo abrir el archivo.\n";
        return;
    }

    Producto temp;
    file.read((char *)&temp, sizeof(Producto)); // Lee el primer producto

    while (!file.eof())
    {
        productos.push_back(temp);
        file.read((char *)&temp, sizeof(Producto));
    }
    file.close();

    // Ordenar por descripción el vector
    for (auto &producto : productos)
    {
        for (int i = 0; producto.description[i]; i++)
        {
            producto.description[i] = toupper(producto.description[i]);
        }
    }
    std::chrono::time_point<std::chrono::high_resolution_clock> instanteInicial, instanteFinal;
    instanteInicial = std::chrono::high_resolution_clock::now();
    quicksort(productos, 0, productos.size() - 1);
    instanteFinal = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> segundos = instanteFinal - instanteInicial;

    for (const auto &producto : productos)
    {
        std::cout << i << ". " << producto.description << "\n";
        i++;
    }
    std::cout << "Ordenamiento terminado.\n";
    std::cout << "Tiempo de ejecucion de Quicksort: " << segundos.count() << " seconds.\n";

    return;
}

int dividirQuicksort(std::vector<Producto> &producto, int izquierda, int derecha)
{
    std::string pivote = producto[derecha].description; // Usa toda la descripción como pivote
    int i = (izquierda - 1);

    for (int j = izquierda; j <= derecha - 1; j++)
    {
        if (producto[j].description <= pivote)
        {
            i++;
            std::swap(producto[i], producto[j]);
        }
    }
    std::swap(producto[i + 1], producto[derecha]);
    return (i + 1);
}

void quicksort(std::vector<Producto> &producto, int izquierda, int derecha)
{
    if (izquierda < derecha)
    {
        int pivote = dividirQuicksort(producto, izquierda, derecha);
        quicksort(producto, izquierda, pivote - 1);
        quicksort(producto, pivote + 1, derecha);
    }
}
