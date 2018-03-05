#include "data_file.h"

data_file::data_file(char* direccion)
{
    this->direccion = new  char[sizeof(direccion)];
    this->archivo = new fstream();
    strcpy(this->direccion, direccion);
}

void data_file::abrir()
{
    this->archivo->open(this->direccion,ios::out | fstream::app);
}

void data_file::cerrar()
{
    this->archivo->close();
}

void data_file::escribir(char *input,int pos,int bytes)
{
    // ofstream out(this->direccion,ios::binary);
    // out.seekp(pos);
    // out.write(input,bytes);
    fstream f(direccion);
    f.seekg(pos);
    f.write(input,bytes);
}

void data_file::escribirFinal(char *input,int bytes)
{
    ofstream out(this->direccion,ios::binary | ios::app | ios::ate );
    out.write(input,bytes);
}

char* data_file::leer(int posicion, int bytes)
{
    char* retorno = new char[bytes];
    ifstream in(this->direccion,ios::in | ios::out | ios::binary);
    in.seekg(posicion);
    in.read(retorno,bytes);
    return retorno;
}

int data_file::getPosFinal()
{
    ifstream in(this->direccion,ios::in | ios::binary);
    in.seekg(0,ios::end);
    return in.tellg();
}