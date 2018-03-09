#include "structs.h"

class ArbolAvl
{
public:
    ArbolAvl(string dirArchivo)
    {
        _raiz = NULL;
        char* arch = new char[strlen(dirArchivo.c_str())];
        strcpy(arch,dirArchivo.c_str());
        file = new data_file(arch);
        ifstream f(dirArchivo.c_str());
        if(f)
        {
            cargarArbol(dirArchivo.c_str());
        }
        else
        {
            guardarArbol(dirArchivo.c_str());
        }

    }
    void prueba()
    {
        int i = 0;
        int cantRegis = (file->getPosFinal()-4)/60;
        cout<<"----------------------------------"<<endl;
        cout<<"Padre actual: "<<getItem(*file,charToInt(file->leer(0,4))).codigo<<endl;
        bool cont = true;
        while(cont)
        {
            item act = getItem(*file,i);
            int izq = act.hijo_izquierdo;
            int der = act.hijo_derecho;
            if(izq != -1)
                izq = getItem(*file,izq).codigo;
            if(der != -1)
                der = getItem(*file,der).codigo;
            cout<<"El "<<act.codigo<<" sus hijos son: izq - "<<izq<<" der - "<<der<<endl;
            i++;

            if(i >= cantRegis)
                cont = false;
        }
    }
    // void agregarNodo(int codigo,char nombre[30],char dpto[15])
    // {
    //     ItemMemory* nuevo = new ItemMemory(codigo,nombre,dpto);
    //     int tmp = _raiz->codigo;
    //     agg(&_raiz,nuevo);
    //     if(this->_raiz->codigo != tmp)
    //     {
    //         int pos = getRegistroPosFromFile(*file,nuevo->codigo);
    //         file->escribir(reinterpret_cast<char*>(&pos),0,4);
    //     }
    //     guardarHijos(*this->file,this->_raiz);
    // }
    void agregarNodo(ItemMemory* nuevo)
    {
        if(_raiz != NULL)
        {
            int cod = _raiz->codigo;
            if(agg(&_raiz,nuevo) == -1)
                return;
            if(cod != _raiz->codigo)
            {
                int pos = getRegistroPosFromFile(*file,_raiz->codigo);
                file->escribir(reinterpret_cast<char*>(&pos),0,4);
            }

        }
        else
        {
            int pos = 0;
            file->escribir(reinterpret_cast<char*>(&pos),0,4);
            agg(&_raiz,nuevo);
        }
        // prueba();
        // guardarHijos(*file,_raiz);
    }
    int buscar(int codigo)
    {
        if(_raiz == NULL)
            return -1;
        return buscar(codigo,&(*_raiz));
    }
    void imprimirPreOrder()
    {
        cout<<"Datos en el arbol[PreOrder]: \n";
        imprimirArbolPre(_raiz);
    }
    void imprimirPosOrder()
    {
        cout<<"Datos en el arbol[PosOrder]: \n";
        imprimirArbolPos(_raiz);
    }
    void imprimirInOrder()
    {
        cout<<"Datos en el arbol[InOrder]: \n";
        imprimirArbolIn(_raiz);
    }
    void cargarArbol(string nameArchivo)
    {
        int posRaiz = charToInt(file->leer(0,4));
        if(posRaiz == -1)
            return;
        item raiz = getItem(*file,posRaiz);
        _raiz = new ItemMemory(raiz.codigo,raiz.nombre,raiz.dpto);
        cargarHijos(raiz,&_raiz);
    }
    void eliminar(int codigo)
    {
        elimi(codigo,&_raiz);
    }
    void guardarArbol(string archivo)
    {
        int posInicial = -1;
        file->abrir();
        file->escribir(reinterpret_cast<char*>(&posInicial),0,sizeof(int));
        if(posInicial == -1)
            return;
        guardarRegistro(*file,_raiz);
        //ahora le asignare la posicion de los hijos izquierdos y derechos en el archivo
        guardarHijos(*file,_raiz);
    }

private:
    ItemMemory* _raiz;
    data_file* file;
    bool existFile(const char *fileName)
    {
        ifstream infile(fileName);
        return infile.good();
    }
    int elimi(int codigo, ItemMemory** sr)
    {
        if((*sr)->codigo == codigo)
        {
            ItemMemory* tmp = *sr;
            if((*sr)->hijo_izquierdo == NULL && (*sr)->hijo_derecho == NULL)
            {
                *sr = NULL;
                ItemMemory* padre = padreDe(_raiz,*sr);
                actualizarHijosFile(padre,padre->hijo_derecho,padre->hijo_izquierdo);
            }
            delete tmp;
        }
        else if((*sr)->codigo > codigo)
        {
            return elimi(codigo,&(*sr)->hijo_izquierdo);
        }
        else if((*sr)->codigo < codigo)
        {
            return elimi(codigo,&(*sr)->hijo_derecho);
        }
    }
    void rotacionDer(ItemMemory** sr)
    {
        ItemMemory* padreSr = padreDe(_raiz,*sr);
        ItemMemory* sigIzq = (*sr)->hijo_izquierdo;
        (*sr)->hijo_izquierdo = sigIzq->hijo_derecho;
        if((*sr)->hijo_izquierdo == NULL)
            (*sr)->alturaIzq = 0;
        else
        {
            (*sr)->alturaIzq = max((*sr)->hijo_izquierdo->alturaIzq,(*sr)->hijo_izquierdo->alturaDer)+1;
        }
        actualizarHijosFile(*sr,(*sr)->hijo_derecho,sigIzq->hijo_derecho);
        sigIzq->hijo_derecho = *sr;
        if(sigIzq->hijo_derecho == NULL)
            sigIzq->alturaDer = 0;
        else
        {
            sigIzq->alturaDer = max(sigIzq->hijo_derecho->alturaIzq,sigIzq->hijo_derecho->alturaDer)+1;
        }
        actualizarHijosFile(sigIzq,*sr,sigIzq->hijo_izquierdo);
        if(padreSr != NULL)
        {
            if(padreSr->hijo_derecho != NULL && padreSr->hijo_derecho->codigo == (*sr)->codigo)
                actualizarHijosFile(padreSr,sigIzq,padreSr->hijo_izquierdo);
            else
                actualizarHijosFile(padreSr,padreSr->hijo_derecho,sigIzq);

        }
        *sr = sigIzq;  
    }
    void rotacionIzq(ItemMemory** sr)
    {
        ItemMemory* padreSr = padreDe(_raiz,*sr);
        ItemMemory* sigDer = (*sr)->hijo_derecho;
        (*sr)->hijo_derecho = sigDer->hijo_izquierdo;
        if((*sr)->hijo_derecho == NULL)
            (*sr)->alturaDer = 0;
        else
        {
            (*sr)->alturaDer = max((*sr)->hijo_derecho->alturaIzq,(*sr)->hijo_derecho->alturaDer)+1;
        }
        actualizarHijosFile(*sr,sigDer->hijo_izquierdo,(*sr)->hijo_izquierdo);
        sigDer->hijo_izquierdo = *sr;
        if(sigDer->hijo_izquierdo == NULL)
            sigDer->alturaIzq = 0;
        else
        {
            sigDer->alturaIzq = max(sigDer->hijo_izquierdo->alturaIzq,sigDer->hijo_izquierdo->alturaDer)+1;
        }
        actualizarHijosFile(sigDer,sigDer->hijo_derecho,*sr);
        if(padreSr != NULL)
        {
            if(padreSr->hijo_derecho != NULL && padreSr->hijo_derecho->codigo == (*sr)->codigo)
                actualizarHijosFile(padreSr,sigDer,padreSr->hijo_izquierdo);
            else
                actualizarHijosFile(padreSr,padreSr->hijo_derecho,sigDer);

        }
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
        {
            actualizarHijosFile(*sr,(*sr)->hijo_derecho,(*sr)->hijo_izquierdo);
            return;
        }
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
    ItemMemory* padreDe(ItemMemory* raiz,ItemMemory* hijo)
    {
        
        if(raiz == NULL || raiz == hijo)
            return NULL;
        if(raiz->hijo_izquierdo == hijo)
            return raiz;
        if(raiz->hijo_derecho == hijo)
            return raiz;
        else
        {
            ItemMemory* padre = padreDe(raiz->hijo_izquierdo,hijo);
            if(padre == NULL)
            {
                return padreDe(raiz->hijo_derecho,hijo);
            }
            return padre;
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
            item itm;
            itm.hijo_derecho -1;
            itm.hijo_izquierdo = -1;
            itm.codigo = nuevo->codigo;
            strcpy(itm.nombre,nuevo->nombre);
            strcpy(itm.dpto,nuevo->dpto);
            file->escribirFinal(reinterpret_cast<char*> (&itm),sizeof(item));
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
    void imprimirArbolPre(ItemMemory* sb)
    {
        if(sb == NULL)
        {
            return;
        }
        cout<<sb->codigo<<",";

        imprimirArbolPre(sb->hijo_izquierdo);
        imprimirArbolPre(sb->hijo_derecho);
    }
    void imprimirArbolPos(ItemMemory* sb)
    {
        if(sb == NULL)
        {
            return;
        }
        imprimirArbolPos(sb->hijo_izquierdo);
        imprimirArbolPos(sb->hijo_derecho);
        cout<<sb->codigo<<",";

    }
    void imprimirArbolIn(ItemMemory* sb)
    {
        if(sb == NULL)
        {
            return;
        }
        imprimirArbolIn(sb->hijo_izquierdo);
        cout<<sb->codigo<<",";
        imprimirArbolIn(sb->hijo_derecho);

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
    void cargarHijos(item raiz,ItemMemory** raizItem)
    {
        if(raiz.hijo_derecho != -1)
        {
            item hijoDer = getItem(*file,raiz.hijo_derecho);
            ItemMemory* raizRetorno = new ItemMemory(hijoDer.codigo,hijoDer.nombre,hijoDer.dpto);
            (*raizItem)->hijo_derecho = raizRetorno;
            cargarHijos(hijoDer,&raizRetorno);
            (*raizItem)->alturaDer = (*raizItem)->alturaDer +1; 
        }
        if(raiz.hijo_izquierdo != -1)
        {
            item hijoIzq = getItem(*file,raiz.hijo_izquierdo);
            ItemMemory* raizRetorno = new ItemMemory(hijoIzq.codigo,hijoIzq.nombre,hijoIzq.dpto);
            (*raizItem)->hijo_izquierdo = raizRetorno;
            cargarHijos(hijoIzq,&raizRetorno);
            (*raizItem)->alturaIzq = (*raizItem)->alturaIzq +1;
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
    void actualizarHijosFile(ItemMemory* sr,ItemMemory* hijo_der = NULL,ItemMemory* hijo_izq = NULL)
    {
        if(sr == NULL)
            return;
        int posInFile = getRegistroPosFromFile(*file,sr->codigo);
        if( posInFile != -1)
        {
            item itm = getItem(*file,posInFile);
            itm.codigo = sr->codigo;
            itm.hijo_derecho = -1;
            itm.hijo_izquierdo = -1;
            memcpy(itm.nombre,sr->nombre,30);
            memcpy(itm.dpto,sr->dpto,15);
            if(hijo_der != NULL)
                itm.hijo_derecho = getRegistroPosFromFile(*file,hijo_der->codigo);
            if(hijo_izq != NULL)                
                itm.hijo_izquierdo = getRegistroPosFromFile(*file,hijo_izq->codigo);
            int setOff = 4+(posInFile*sizeof(item));
            file->escribir(reinterpret_cast<char*>(&itm),setOff,sizeof(item));
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