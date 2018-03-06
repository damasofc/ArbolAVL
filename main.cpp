#include "arbolAvl.h"
 
using namespace std;

int main()
{
    ItemMemory* raiz = new ItemMemory(43,"Damaso","Sistemas");
    ItemMemory* raiz2 = new ItemMemory(18,"Gabriel","admin");
    ItemMemory* raiz3 = new ItemMemory(22,"Nelson","admin");
    ItemMemory* raiz4 = new ItemMemory(9,"Iris","conta");
    // ItemMemory* raiz5 = new ItemMemory(5,"Jaun","conta");
    //ItemMemory* raiz5 = new ItemMemory(5,"Jaun","conta");
    ArbolAvl* arbol = new ArbolAvl();
    // arbol->cargarArbol("arbol2.dat");
    arbol->agregarNodo(raiz);
    arbol->agregarNodo(raiz2);
    arbol->agregarNodo(raiz3);
    arbol->agregarNodo(raiz4);
    // arbol->agregarNodo(raiz5);
    // arbol->guardarArbol("arbol2.dat");
    // arbol->cargarArbol("arbolRotDoble.dat");
    arbol->imprimir();

    return 0;
}

