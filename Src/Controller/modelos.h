#ifndef MODELO_H
#define MODELO_H

#include <iostream>
#include <stdlib.h>
#include <cstring>

struct Producto
{
    int id = -1;
    int id_proveedor = -1;
    int stock = -1;
    float precio = -1;
    char description[45] = "";
    int stock_min = -1;
};

struct Cliente
{
    int id = -1;
    char nombre[45] = "";
    char direccion[70] = "";
    char telefono[18] = "";
};

#endif