#include "structs.h"

class ArbolAvl
{
public:
    ArbolAvl()
    {
        _raiz = NULL;
    }
    int agregarNodo(int codigo,char nombre[30],char dpto[15])
    {
        ItemMemory* nuevo = new ItemMemory(codigo,nombre,dpto);
        return agg(&_raiz,nuevo);
    }
    int agregarNodo(ItemMemory* nuevo)
    {
        return agg(&_raiz,nuevo);
    }
    int buscar(int codigo)
    {
        if(_raiz == NULL)
            return -1;
        return buscar(codigo,&(*_raiz));
    }
    void imprimir()
    {
        cout<<"Datos en el arbol: \n";
        imprimirArbol(_raiz);
    }
    void cargarArbol(string nameArchivo)
    {
        char* arch = new char(strlen(nameArchivo.c_str()));
        strcpy(arch,nameArchivo.c_str());
        data_file f(arch);
        int posRaiz = charToInt(f.leer(0,4));
        item raiz = getItem(f,posRaiz);
        _raiz = new ItemMemory(raiz.codigo,raiz.nombre,raiz.dpto);
        cargarHijos(f,raiz,&_raiz);
    }
    void guardarArbol(string archivo)
    {
        char* name = new char[sizeof(archivo)];
        strcpy(name,archivo.c_str());
        data_file file(name);
        int posInicial = 0;
        file.abrir();
        file.escribir(reinterpret_cast<char*>(&posInicial),0,sizeof(int));
        guardarRegistro(file,_raiz);
        //ahora le asignare la posicion de los hijos izquierdos y derechos en el archivo
        guardarHijos(file,_raiz);
    }

private:
    ItemMemory* _raiz;
    void rotacionDer(ItemMemory** sr)
    {
        ItemMemory* sigIzq = (*sr)->hijo_izquierdo;
        (*sr)->hijo_izquierdo = sigIzq->hijo_derecho;
        if((*sr)->hijo_izquierdo == NULL)
            (*sr)->alturaIzq = 0;
        else
            (*sr)->alturaIzq = max((*sr)->hijo_izquierdo->alturaIzq,(*sr)->hijo_izquierdo->alturaDer);
        sigIzq->hijo_derecho = *sr;
        if(sigIzq->hijo_derecho == NULL)
            sigIzq->alturaDer = 0;
        else
            sigIzq->alturaDer = max(sigIzq->hijo_derecho->alturaIzq,sigIzq->hijo_derecho->alturaDer);
        *sr = sigIzq;    
    }
    void rotacionIzq(ItemMemory** sr)
    {
        ItemMemory* sigDer = (*sr)->hijo_derecho;
        (*sr)->hijo_derecho = sigDer->hijo_izquierdo;
        if((*sr)->hijo_derecho == NULL)
            (*sr)->alturaDer = 0;
        else
            (*sr)->alturaDer = max((*sr)->hijo_derecho->alturaIzq,(*sr)->hijo_derecho->alturaDer);
        sigDer->hijo_izquierdo = *sr;
        if(sigDer->hijo_izquierdo == NULL)
            sigDer->alturaIzq = 0;
        else
            sigDer->alturaDer = max(sigDer->hijo_izquierdo->alturaIzq,sigDer->hijo_izquierdo->alturaDer);
        *sr = sigDer;    
    }
    void rotDobleIzq(ItemMemory** sr)
    {
        rotacionDer(&(*sr)->hijo_derecho);
        rotacionIzq(&(*sr));
    }
    void rotDobleDer(ItemMemory** sr)
    {
        rotacionIzq(&(*sr)->hijo_izquierdo);
        rotacionDer(&(*sr));
    }
    void balancear(ItemMemory** sr)
    {
        if(abs((*sr)->alturaIzq - (*sr)->alturaDer) < 2)
            return;
        if((*sr)->alturaDer > (*sr)->alturaIzq)
        {
            if((*sr)->hijo_derecho->alturaDer >= (*sr)->hijo_derecho->alturaIzq)
            {
                rotacionIzq(sr);
            }
            else
            {
                rotDobleIzq(sr);
            }
        }
        else
        {
            if((*sr)->hijo_izquierdo->alturaIzq >= (*sr)->hijo_izquierdo->alturaDer)
            {
                rotacionDer(sr);
            }
            else
            {
                rotDobleDer(sr);
            }
        }
    }
    int abs(int x)
    {
        if(x < 0)
            return x*-1;
        return x;
    }
    int max(int x, int y)
    {
        if(x>y)
            return x;
        return y;
    }
    int agg(ItemMemory** raiz,ItemMemory* nuevo)
    {
        if(*raiz == NULL)
        {
            *raiz = nuevo;
            return 0;
        }
        if((*raiz)->codigo > nuevo->codigo)
        {
            (*raiz)->alturaIzq = agg(&(*raiz)->hijo_izquierdo,nuevo)+1;
            balancear(raiz);
            return max((*raiz)->alturaDer,(*raiz)->alturaIzq);
        }
        if((*raiz)->codigo < nuevo->codigo)
        {
            (*raiz)->alturaDer = agg(&(*raiz)->hijo_derecho,nuevo)+1;
            balancear(raiz);
            return max((*raiz)->alturaDer,(*raiz)->alturaIzq);
        }
        return -1;
    }
    int buscar(int codigo,ItemMemory* raiz)
    {
        if(raiz == NULL)
            return -1;
        if(raiz->codigo == codigo)
            return codigo;
        if(codigo > raiz->codigo)
            return buscar(codigo,raiz->hijo_derecho);
        return buscar(codigo,raiz->hijo_izquierdo);
    }
    void imprimirArbol(ItemMemory* sb)
    {
        if(sb == NULL)
        {
            return;
        }
        cout<<"***************"<<endl;
        cout<<sb->codigo<<endl;
        cout<<sb->nombre<<endl;
        cout<<sb->dpto<<endl;
        imprimirArbol(sb->hijo_izquierdo);
        imprimirArbol(sb->hijo_derecho);
    }
    int charToInt(char* x)
    {
        int i = (x[3] << 24) | (x[2] << 16) | (x[1] << 8) | (x[0]);
        return i;
    }
    item getItem(data_file file,int pos)
    {
        item retorno;
        int posPuntero = sizeof(int) + (pos*sizeof(item));
        retorno.codigo = charToInt(file.leer(posPuntero,sizeof(int)));
        // cout<<charToInt(file.leer(64,sizeof(int)))<<endl;
        posPuntero+=sizeof(int);
        memcpy(retorno.nombre,file.leer(posPuntero,sizeof(retorno.nombre)),sizeof(retorno.nombre));
        posPuntero+= sizeof(retorno.nombre);
        memcpy(retorno.dpto,file.leer(posPuntero,sizeof(retorno.dpto)),sizeof(retorno.dpto));
        //ese +3 se coloca porque toma 3 bytes mas como un espacio que crea el S.O.
        posPuntero+= sizeof(retorno.dpto)+3;
        retorno.hijo_derecho = charToInt(file.leer(posPuntero,sizeof(int)));
        posPuntero+=sizeof(int);
        retorno.hijo_izquierdo = charToInt(file.leer(posPuntero,sizeof(int)));
        return retorno;

    }
    void cargarHijos(data_file f,item raiz,ItemMemory** raizItem)
    {
        if(raiz.hijo_derecho != -1)
        {
            item hijoDer = getItem(f,raiz.hijo_derecho);
            ItemMemory* raizRetorno = new ItemMemory(hijoDer.codigo,hijoDer.nombre,hijoDer.dpto);
            (*raizItem)->hijo_derecho = raizRetorno;
            cargarHijos(f,hijoDer,&raizRetorno);
        }
        if(raiz.hijo_izquierdo != -1)
        {
            item hijoIzq = getItem(f,raiz.hijo_izquierdo);
            ItemMemory* raizRetorno = new ItemMemory(hijoIzq.codigo,hijoIzq.nombre,hijoIzq.dpto);
            (*raizItem)->hijo_izquierdo = raizRetorno;
            cargarHijos(f,hijoIzq,&raizRetorno);
        }    
    }
    bool guardarRegistro(data_file file,ItemMemory* raiz)
    {
        if(raiz == NULL)
        {
            return false;
        }
        else
        {
            item itm;
            itm.codigo = raiz->codigo;
            strcpy(itm.nombre,raiz->nombre);
            strcpy(itm.dpto,raiz->dpto);
            file.escribirFinal(reinterpret_cast<char*> (&itm),sizeof(item));
            guardarRegistro(file,raiz->hijo_izquierdo);
            guardarRegistro(file,raiz->hijo_derecho);
            return true;
        }
    }
    void guardarHijos(data_file file,ItemMemory* raiz)
    {
        if(raiz == NULL)
        {
            return;
        }
        else
        {
            int posInFile = getRegistroPosFromFile(file,raiz->codigo);
            if( posInFile != -1)
            {
                item itm;
                itm.codigo = raiz->codigo;
                memcpy(itm.nombre,raiz->nombre,30);
                memcpy(itm.dpto,raiz->dpto,15);
                if(raiz->hijo_derecho != NULL)
                    itm.hijo_derecho = getRegistroPosFromFile(file,raiz->hijo_derecho->codigo);
                if(raiz->hijo_izquierdo != NULL)                
                    itm.hijo_izquierdo = getRegistroPosFromFile(file,raiz->hijo_izquierdo->codigo);
                int setOff = 4+(posInFile*sizeof(item));
                file.escribir(reinterpret_cast<char*>(&itm),setOff,sizeof(item));
                guardarHijos(file,raiz->hijo_izquierdo);
                guardarHijos(file,raiz->hijo_derecho);
            }
        }
    }
    int getRegistroPosFromFile(data_file file,int codigo)
    {
        if(codigo == -1)
            return -1;
        int posPuntero = 4;
        int pos = 0;
        int posFinal = file.getPosFinal();
        while(posPuntero <= posFinal)
        {
            int codi = charToInt(file.leer(posPuntero,4));
            if(codi == codigo)
                return pos;
            pos++;
            posPuntero+= sizeof(item);
        }
        return -1;
    }
};