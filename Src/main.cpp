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
void distribuir(std::string f, std::string f1, std::string f2, int longSec, int numReg);
void subSecuencia(std::ifstream &f, std::ofstream &t, int longSec);
void mezclar(std::string f1, std::string f2, std::string f, int lonSec, int numReg);

// Método por Mezcla Natural

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
    int longSec;

    // Abrimos el archivo original para duplicarlo en el auxiliar
    std::ifstream file("../Database/Cliente.bin", std::ios::binary | std::ios::ate);
    std::fstream ClienteOrdenadoAux("../Database/ClienteOrdenado.bin", std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);
    if (!file || !ClienteOrdenadoAux)
    {
        std::cout << "No se pudo abrir uno de los archivos temporales.\n";
        return;
    }

    // Calculamos el número de registros
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    int numEstructuras = size / sizeof(Cliente);

    // Cambiamos los nombres a mayúsculas para las comparaciones ASCII usando un buffer
    Cliente *buffer = new Cliente[numEstructuras];
    file.read((char *)buffer, numEstructuras * sizeof(Cliente));
    for (int i = 0; i < numEstructuras; i++)
    {
        for (int j = 0; buffer[i].nombre[j]; j++)
        {
            buffer[i].nombre[j] = toupper(buffer[i].nombre[j]);
        }
    }

    // Luego guardamos las estructuras corregidas en el archivo auxiliar, eliminamos el buffer y cerramos el archivo copia
    ClienteOrdenadoAux.write((char *)buffer, (numEstructuras) * sizeof(Cliente));
    delete[] buffer;
    ClienteOrdenadoAux.close();

    // Empezamos el ordenamiento en 1
    longSec = 1;

    // Tomamos el tiempo antes de empezar
    struct timeval t, t2;
    int microsegundos;
    gettimeofday(&t, NULL);
    // Revisamos que hayan estructucturas en el binario y con cada iteración vamos haciendo divisiones longSec más grandes
    while (longSec < numEstructuras)
    {
        distribuir("../Database/ClienteOrdenado.bin", "../Database/Izquierda.bin", "../Database/Derecha.bin", longSec, numEstructuras);
        mezclar("../Database/Izquierda.bin", "../Database/Derecha.bin", "../Database/ClienteOrdenado.bin", longSec, numEstructuras);
        longSec *= 2;
    }
    // Tomamos el tiempo al terminar y calculamos
    gettimeofday(&t2, NULL);
    microsegundos = ((t2.tv_usec - t.tv_usec) + ((t2.tv_sec - t.tv_sec) * 1000000.0f));
    registro.listarClientes("../Database/ClienteOrdenado.bin");
    std::cout << "Ordenamiento por Mezcla Directa Terminado.\n";
    printf("Tiempo de ejecucion de Quicksort: %.16g microsegundos.\n", microsegundos);
}

void distribuir(std::string f, std::string f1, std::string f2, int longSec, int numReg)
{
    int numSec, resto, i;

    // Abrimos bf (copia) en lectura, y los dos auxiliares (bf y pw1) en escritura
    std::ifstream bf(f, std::ios::binary);
    std::ofstream pw1(f1, std::ios::binary), pw2(f2, std::ios::binary);

    // longSec es la longitud de una de las mitades, por eso multiplicamos por 2 y calculamos cuántas secciones de ese tamaño se pueden formar en el archivo
    numSec = numReg / (2 * longSec);
    // Calculamos si hay algún sobrante en estas particiones para manejarlas por separado
    resto = numReg % (2 * longSec);

    // Accedemos a cada división con los dos archivos auxiliares
    for (i = 1; i <= numSec; i++)
    {
        subSecuencia(bf, pw1, longSec);
        subSecuencia(bf, pw2, longSec);
    }

    // Manejamos el resto, si se puede hacer una seccion completa se guarda en f1 y el restante en f2
    if (resto > longSec)
        resto -= longSec;
    else
    {
        longSec = resto;
        resto = 0;
    }

    subSecuencia(bf, pw1, longSec);
    subSecuencia(bf, pw2, resto);
}

void subSecuencia(std::ifstream &f, std::ofstream &t, int longSec)
{
    // Escribimos las divisiones del archivo principal en el archivo auxiliar recibido correspondiente
    Cliente cliente;
    for (int j = 1; j <= longSec; j++)
    {
        if (f.read((char *)&cliente, sizeof(cliente)))
        {
            t.write((char *)&cliente, sizeof(cliente));
        }
    }
}

void mezclar(std::string f1, std::string f2, std::string f, int lonSec, int numReg)
{
    int numSec, resto, i, j, k;
    Cliente cliente1, cliente2;

    // Nuevamente hacemos particiones para mezclar mitades entre los dos auxiliares
    numSec = numReg / (2 * lonSec);
    resto = numReg % (2 * lonSec);

    // Abrimos los archivos nuevamente (si no se abren y cierran tras cada función la información no se guardará correctamente)
    std::ifstream bf1(f1, std::ios::binary), bf2(f2, std::ios::binary);
    std::ofstream pw(f, std::ios::binary);

    // Leemos el primer registro de cada archivo auxiliar y los guardamos en las estructuras de tipo Cliente
    bf1.read((char *)&cliente1, sizeof(cliente1));
    bf2.read((char *)&cliente2, sizeof(cliente2));

    // Se comparan las estructuras de los archivos auxiliares en bloques, y se van escribiendo con el nuevo orden en el archivo copia
    for (int s = 1; s <= numSec + 1; s++)
    {
        int n1, n2;
        n1 = n2 = lonSec;
        if (s == numSec + 1)
        {
            if (resto > lonSec)
                n2 = resto - lonSec;
            else
            {
                n1 = resto;
                n2 = 0;
            }
        }
        i = j = 1;
        while (i <= n1 && j <= n2)
        {
            Cliente cliente;
            if (std::string(cliente1.nombre) < std::string(cliente2.nombre))
            {
                cliente = cliente1;
                if (bf1.read((char *)&cliente1, sizeof(cliente1)))
                {
                    cliente1 = cliente1;
                }
                i++;
            }
            else
            {
                cliente = cliente2;
                if (bf2.read((char *)&cliente2, sizeof(cliente2)))
                {
                    cliente2 = cliente2;
                }
                j++;
            }
            pw.write((char *)&cliente, sizeof(cliente));
        }

        for (k = i; k <= n1; k++)
        {
            pw.write((char *)&cliente1, sizeof(cliente1));
            if (bf1.read((char *)&cliente1, sizeof(cliente1)))
            {
                cliente1 = cliente1;
            }
        }
        for (k = j; k <= n2; k++)
        {
            pw.write((char *)&cliente2, sizeof(cliente2));
            if (bf2.read((char *)&cliente2, sizeof(cliente2)))
            {
                cliente2 = cliente2;
            }
        }
    }
}
