#include "auxiliares.h"

int auxiliares::validarInt(int maximo)
{
    int numero;
    do
    {
        while (!(std::cin >> numero))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida. Por favor, ingresa un numero: ";
        }
        if (numero <= 0 || numero > maximo)
        {
            std::cout << "Fuera de las opciones del menu, introduzca otro numero.\n";
        }
    } while (numero <= 0 || numero > maximo);
    return numero;
}