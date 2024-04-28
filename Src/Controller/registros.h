#ifndef REGISTRO_H
#define REGISTRO_H

#include <iostream>
#include <stdlib.h>
#include <limits>
#include <locale.h>
#include <fstream>
#include <cstring>
#include <string>

class registros
{

public:
    void listarClientes(std::string direccion);
    void listarProductos();
};

#endif