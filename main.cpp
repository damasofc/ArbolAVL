#include "arbolAvl.h"
 
using namespace std;

int main()
{
    ItemMemory* raiz = new ItemMemory(43,"Damaso","Sistemas");
    ItemMemory* raiz2 = new ItemMemory(18,"Gabriel","admin");
    ItemMemory* raiz3 = new ItemMemory(22,"Nelson","admin");
    ItemMemory* raiz4 = new ItemMemory(9,"Iris","conta");
    ItemMemory* raiz5 = new ItemMemory(21,"Iris","conta");
    ItemMemory* raiz6 = new ItemMemory(6,"Iris","conta");
    ItemMemory* raiz7 = new ItemMemory(8,"Iris","conta");
    ItemMemory* raiz8 = new ItemMemory(20,"Iris","conta");
    ItemMemory* raiz9 = new ItemMemory(63,"Iris","conta");
    ItemMemory* raiz10 = new ItemMemory(50,"Iris","conta");
    ItemMemory* raiz11 = new ItemMemory(62,"Iris","conta");
    ItemMemory* raiz12 = new ItemMemory(51,"Iris","conta");
    ItemMemory* raiz13 = new ItemMemory(65,"Jaun","conta");
    ItemMemory* raiz14 = new ItemMemory(45,"Jaun","conta");
    ArbolAvl* arbol = new ArbolAvl("arbol.dat");
    // arbol->agregarNodo(raiz);
    // arbol->agregarNodo(raiz2);
    // arbol->agregarNodo(raiz3);
    // arbol->agregarNodo(raiz4);

    // arbol->agregarNodo(raiz5);
    // arbol->agregarNodo(raiz6);
    // arbol->agregarNodo(raiz7);
    // arbol->agregarNodo(raiz8);
    // arbol->agregarNodo(raiz9);
    // arbol->agregarNodo(raiz10);
    // arbol->agregarNodo(raiz11);
    // arbol->agregarNodo(raiz12);
    // arbol->agregarNodo(raiz13);
    // arbol->agregarNodo(raiz14);
    arbol->imprimirPreOrder();
    arbol->eliminar(43);
    /**
     Ya probe el de alturas al cargar el arbol, al agregar un nuevo
     Nodo, que se actualiza en el file y en el arbol en memoria.
     me falta probar el eliminar que todo este correcto.
    **/
    arbol->imprimirPreOrder();

    return 0;
}

