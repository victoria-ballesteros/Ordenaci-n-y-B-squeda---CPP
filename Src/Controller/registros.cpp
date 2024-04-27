#include "registros.h"
#include "modelos.h"

void registros::listarClientes()
{
    int contador = 1;
    std::fstream archivo("../Database/Cliente.bin", std::ios::binary | std::ios::in | std::ios::out);
    if (archivo.fail())
    {
        std::cerr << "No se pudo abrir el archivo.\n";
        system("pause");
        return;
    }
    Cliente cliente;
    while (archivo.read((char *)&cliente, sizeof(Cliente)))
    {
        std::cout << contador << ".\n";
        std::cout << "ID--------------------: " << cliente.id << "\n";
        std::cout << "NOMBRE----------------: " << cliente.nombre << "\n";
        std::cout << "__________________________________________________\n";
        contador++;
    }
    archivo.close();
}

void registros::listarProductos()
{
}