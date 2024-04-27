#include <iostream>
#include <stdlib.h>
#include <cstring>

#include "auxiliares.h"
#include "Controller/Controller.h"

// Instancias externas
auxiliares auxiliar;
registros registro;

int main()
{
    int opcionMenuPrincipal = 0, opcionMenuSecundario = 0;

    std::cout << "1. Mostrar Registros Binarios\n";
    std::cout << "2. Ordenar Registros Binarios\n";
    opcionMenuPrincipal = auxiliar.validarInt(2);

    if (opcionMenuPrincipal == 1)
    {
        std::cout << "1. Imprimir Registros: Cliente.bin\n";
        std::cout << "2. Imprimir Registros: Producto.bin\n";
        opcionMenuSecundario = auxiliar.validarInt(1);
        if (opcionMenuSecundario == 1)
        {
            registro.listarClientes();
        }
        else if (opcionMenuSecundario == 2)
        {
        }
    }
    else if (opcionMenuPrincipal == 2)
    {
        std::cout << "1. Ordenar Alfabeticamente: Cliente.bin\n";
        opcionMenuSecundario = auxiliar.validarInt(1);
    }
}