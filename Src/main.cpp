#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <cctype>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <time.h>
#include <sys/time.h>
#include <algorithm>

#include "auxiliares.h"
#include "Controller/Controller.h"

// Instancias externas
auxiliares auxiliar;
registros registro;

// Método Quicksort
void ordenamientoQuicksort();
int dividirQuicksort(std::vector<Producto> &producto, int izquierda, int derecha);
void quicksort(std::vector<Producto> &producto, int izquierda, int derecha);

// Método por Mezcla Directa
void ordenamientoMezclaDirecta();
void mergeSort(std::ifstream &file, std::fstream &ClienteOrdenado, int inicio, int final);
void merge(std::ifstream &file, std::fstream &ClienteOrdenado, int inicio, int mitad, int final);

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
            registro.listarClientes("../Database/Cliente.bin");
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
        std::cout << "2. Ordenar Alfabeticamente (Por Mezcla Directa): Cliente.bin\n";
        opcionMenuSecundario = auxiliar.validarInt(2);

        if (opcionMenuSecundario == 1)
        {
            ordenamientoQuicksort();
        }
        else if (opcionMenuSecundario == 2)
        {
            ordenamientoMezclaDirecta();
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

    // Tomar el tiempo de ejecución
    struct timeval t, t2;
    int microsegundos;
    gettimeofday(&t, NULL);
    quicksort(productos, 0, productos.size() - 1);
    gettimeofday(&t2, NULL);
    microsegundos = ((t2.tv_usec - t.tv_usec) + ((t2.tv_sec - t.tv_sec) * 1000000.0f));
    for (const auto &producto : productos)
    {
        std::cout << i << ". " << producto.description << "\n";
        i++;
    }
    std::cout << "Ordenamiento terminado.\n";
    printf("Tiempo de ejecucion de Quicksort: %.16g microsegundos.\n", microsegundos);

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

void dividirArchivo()
{
    std::ifstream file("../Database/Cliente.bin", std::ios::binary | std::ios::ate);
    if (!file)
    {
        std::cout << "No se pudo abrir uno de los archivos temporales.\n";
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    int numEstructuras = size / sizeof(Cliente);
    // std::cout << numEstructuras << "\n";
    Cliente *buffer = new Cliente[numEstructuras];
    file.read((char *)buffer, numEstructuras * sizeof(Cliente));

    std::fstream izquierda("../Database/Izquierda.bin", std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);
    std::fstream derecha("../Database/Derecha.bin", std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);

    if (!izquierda || !derecha)
    {
        std::cout << "No se pudo abrir uno de los archivos temporales.\n";
        delete[] buffer;
        return;
    }

    izquierda.write((char *)buffer, (numEstructuras / 2) * sizeof(Cliente));
    registro.listarClientes("../Database/Izquierda.bin");
    std::cout << "Primera mitad\n\n";
    derecha.write((char *)(buffer + numEstructuras / 2), (numEstructuras / 2) * sizeof(Cliente));
    registro.listarClientes("../Database/Derecha.bin");
    std::cout << "Segunda mitad\n\n";

    delete[] buffer;
}

void ordenamientoMezclaDirecta()
{
    std::ifstream file("../Database/Cliente.bin", std::ios::binary | std::ios::ate);
    std::fstream ClienteOrdenado("../Database/ClienteOrdenado.bin", std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);
    if (!file || !ClienteOrdenado)
    {
        std::cout << "No se pudo abrir uno de los archivos temporales.\n";
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    int numEstructuras = size / sizeof(Cliente);
    // std::cout << numEstructuras << "\n";
    Cliente *buffer = new Cliente[numEstructuras];
    file.read((char *)buffer, numEstructuras * sizeof(Cliente));
    for (int i = 0; i < numEstructuras; i++)
    {
        for (int j = 0; buffer[i].nombre[j]; j++)
        {
            buffer[i].nombre[j] = toupper(buffer[i].nombre[j]);
        }
    }
    ClienteOrdenado.write((char *)buffer, (numEstructuras) * sizeof(Cliente));
    delete[] buffer;

    // registro.listarClientes("../Database/ClienteOrdenado.bin");
    mergeSort(file, ClienteOrdenado, 0, numEstructuras - 1);
    // registro.listarClientes("../Database/ClienteOrdenado.bin");
    std::cout << "Terminado\n";
}

void mergeSort(std::ifstream &file, std::fstream &ClienteOrdenado, int inicio, int final)
{
    if (inicio < final)
    {
        int mitad = inicio + (final - inicio) / 2;
        // std::cout << "Terminado1\n";
        mergeSort(file, ClienteOrdenado, inicio, mitad);
        //  std::cout << "Terminado\n";
        mergeSort(file, ClienteOrdenado, mitad + 1, final);
        //  std::cout << "Terminado3\n";
        merge(file, ClienteOrdenado, inicio, mitad, final);
        // merge(file, ClienteOrdenado, mitad + 1, mitad / 2, final);
    }
}
void merge(std::ifstream &file, std::fstream &ClienteOrdenado, int inicio, int mitad, int final)
{
    int i, j, k;
    int elementosIzquierda = mitad - inicio + 1;
    int elementosDerecha = final - mitad;
    // Dividir el binario en dos partes
    Cliente *buffer = new Cliente[final];

    ClienteOrdenado.read((char *)buffer, final * sizeof(Cliente));
    registro.listarClientes("../Database/ClienteOrdenado.bin");
    // for (int i = 0; i < final; i++)
    // {
    //     for (int j = 0; buffer[i].nombre[j]; j++)
    //     {
    //         buffer[i].nombre[j] = toupper(buffer[i].nombre[j]);
    //     }
    // }
    std::fstream izquierda("../Database/Izquierda.bin", std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);
    std::fstream derecha("../Database/Derecha.bin", std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);

    // ClienteOrdenado.write((char *)buffer, (final) * sizeof(Cliente));
    //  registro.listarClientes("../Database/ClienteOrdenado.bin");

    if (!izquierda || !derecha)
    {
        std::cout << "No se pudo abrir uno de los archivos temporales.\n";
        delete[] buffer;
        return;
    }
    izquierda.write((char *)buffer, (final / 2) * sizeof(Cliente));
    // registro.listarClientes("../Database/Izquierda.bin");
    // std::cout << "Primera mitad\n\n";
    derecha.write((char *)(buffer + final / 2), (final / 2) * sizeof(Cliente));
    // registro.listarClientes("../Database/Derecha.bin");
    // std::cout << "Segunda mitad\n\n";
    delete[] buffer;

    // Método
    i = 0;
    j = 0;
    k = inicio;
    Cliente Izquierda, Derecha;
    while (i < elementosIzquierda && j < elementosDerecha)
    {
        izquierda.seekg(i * sizeof(Cliente), std::ios::beg);
        izquierda.read((char *)&Izquierda, sizeof(Cliente));
        derecha.seekg(j * sizeof(Cliente), std::ios::beg);
        derecha.read((char *)&Derecha, sizeof(Cliente));
        if (Izquierda.nombre <= Derecha.nombre)
        {
            ClienteOrdenado.seekp(k * sizeof(Cliente));
            ClienteOrdenado.write((char *)&Izquierda, sizeof(Cliente));
            i++;
        }
        else
        {
            ClienteOrdenado.seekp(k * sizeof(Cliente));
            ClienteOrdenado.write((char *)&Derecha, sizeof(Cliente));
            j++;
        }
        k++;
    }

    derecha.seekg(j * sizeof(Cliente), std::ios::beg);
    derecha.read((char *)&Derecha, sizeof(Cliente));
    while (j < elementosDerecha)
    {
        ClienteOrdenado.write((char *)&Derecha, sizeof(Cliente));
        j++;
        k++;
    }
    izquierda.seekg(i * sizeof(Cliente), std::ios::beg);
    izquierda.read((char *)&Izquierda, sizeof(Cliente));
    while (i < elementosIzquierda)
    {
        ClienteOrdenado.write((char *)&Izquierda, sizeof(Cliente));
        i++;
        k++;
    }

    izquierda.close();
    derecha.close();
}
