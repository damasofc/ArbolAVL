#include "arbolAvl.h"
 
using namespace std;

int main()
{
    ItemMemory* raiz = new ItemMemory(100,"Damaso","Sistemas");
    ItemMemory* raiz2 = new ItemMemory(2,"Gabriel","admin");
    ItemMemory* raiz3 = new ItemMemory(35,"Nelson","admin");
    ItemMemory* raiz4 = new ItemMemory(40,"Iris","conta");
    ItemMemory* raiz5 = new ItemMemory(5,"Jaun","conta");
    //ItemMemory* raiz5 = new ItemMemory(5,"Jaun","conta");
    ArbolAvl* arbol = new ArbolAvl();
    arbol->agregarNodo(raiz);
    arbol->agregarNodo(raiz2);
    arbol->agregarNodo(raiz3);
    arbol->agregarNodo(raiz4);
    arbol->agregarNodo(raiz5);
    //arbol->cargarArbol("arbolRotDoble.dat");
    arbol->imprimir();

    return 0;
}

