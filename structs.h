#include <iostream>
#include <string.h>
#include "data_file.h"

struct item{
    int codigo = -1;
    char nombre[30];
    char dpto[15];
    int hijo_derecho = -1;
    int hijo_izquierdo = -1;
};

class ItemMemory{
public:    
    int codigo;
    char nombre[30];
    char dpto[15];
    int alturaIzq;
    int alturaDer;
    ItemMemory* hijo_derecho;
    ItemMemory* hijo_izquierdo;
    ItemMemory(int codigo,char nombre[30],char dpto[15])
    {
        strcpy(this->nombre,nombre);
        strcpy(this->dpto,dpto);
        this->codigo = codigo;
        hijo_derecho = NULL;
        hijo_izquierdo = NULL;
        alturaDer = 0;
        alturaIzq = 0;
    }

};