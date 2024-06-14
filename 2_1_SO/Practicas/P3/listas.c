
#include "listas.h"


bool createNode(tPosL* p) { //Con esta funcion se verifica si hay memoria para crear otro nodo
    *p = malloc(sizeof(struct tNode)); //Recibimos el valor del nodo p, por lo que con punteros reservamos memoria con tamaño de tNode, que es un tPosL
    if (*p != LNULL) { //Verificamos si se creo correctamente para inicializar el char comand e indicar que el siguiente es nulo
        (*p)->comand = LNULL;
        (*p)->next = LNULL;
    }
    return *p != LNULL; //Al final devuelve un booleano indicando si se pudo crear el nodo o no
}

void createList(tList *L){
    *L = LNULL; //Para crear una lista vacia, indicamos que el primer elemento es NULL
}

bool isEmptyList(tList L){
    return(L == LNULL); //Si la lista apunta a NULL, no hay ningun nodo en la lis<<<ta
}

bool insertElement(tData d, tList *L) {
    tPosL p, q; //p actuara como el nodo a añadir a la lista y q en cierto caso se usara como auxiliar

    if (!createNode(&p)) { //Reserva memoria al nodo
        return false; //Si falla al reservar devuelve que no es posible
    } else {
        p->comand = strdup(d); //strdup se usa para evitar errores de asignacion, ya que strcpy nos daba error
        p->next = LNULL; //Queremos que el siguiente sea nulo, ya que se va a poner al final de la lista siempre
        if (isEmptyList(*L)) { //Caso sencillo: Si la lista esta vacia, hacemos que la lista apunte al nodo
            *L = p;
        } else { //Caso mas complicado: Avanzamos q (Auxiliar) hasta el final para que actue como el ultimo nodo para hacer que apunte a p (Que se convertira en el nuevo ultimo nodo)
            for (q = *L; q->next != LNULL; q = q->next); // Movemos al final de la lista
            q->next = p;
        }
        return true; //Confirma su insercion
    }
}


tData getElement(char *charpos, tList L) {
    tPosL p = L;
    int posicion = (int)strtol(charpos, NULL, 10); //Nos servira para obtener el numero de la instruccion a buscar

    if (posicion > 0) {
        if(isEmptyList(L)) //Si esta vacia no hay nada que buscar
            return "Lista vacia, no hay elementos\n";
        for(int i = 1; i != posicion; i++, p = p->next){ //Si no esta vacia avanzamos por la lista con ayuda de un nodo auxiliar p
            if(p->next == LNULL){ //Esto es en caso de llegar al final de la lista en caso de haber introducido un numero superior a la cantidad de nodos
                return "Not found\n";
            }
        }

        return p->comand; //Si encuentra la posicion devuelve el string comand
    } else {
        return "Argument not recognized"; //En caso de haber introducido una cadena invalida o si hubo un error y no se detecto el entero correctamente
    }
}

void PrintList(tList L){
    tPosL p = L; //Necesitamos un nodo que recorra toda la lista, asi que hacemos que apunte al primer nodo. Para eso hacemos que apunte al mismo lugar que la lista
    int i = 1; //Contador del numero del comando (O numero del nodo)
    if (isEmptyList(L) == true) { //Si esta vacia no hay nada que imprimir
        printf("History empty\n");
    } else {
        while (p != LNULL) { //Mientras el nodo no sea nulo (El siguiente al ultimo), que recorra toda la lista
            printf("COMANDO %d: %s", i, p->comand);
            p = p->next; //Hacemos que el nodo avance una posicion
            i++;
        }
    }
}


void PrintNList(int N, tList L){
    tPosL p = L;    //Esta funcion actua igual que PrintList, solo tiene 1 cambio
    int i=1;
    if(isEmptyList(L)==true){
        printf("Lista vacia. No hay nada que imprimir\n");
    } else {
        while (p != NULL && i<=N) { //Aqui esta la unica diferencia, ya que agregamos un codicional para saber si llegamos al numero maximo de nodos a imprimir
            printf("COMANDO %d: %s\n", i, p->comand);
            p = p->next;
            i++;
        }
    }
}

void deleteList(tList *L) {
    tPosL p = *L, next; //Necesitamos dos nodos, uno que vaya liberando la memoria del nodo y otro que avance por la lista

    while (p != LNULL) { //Mientras el nodo no sea nulo (El siguiente al ultimo) que siga por la lista
        next = p->next; //Guardamos la posicion del siguiente nodo en next
        free(p->comand);  //Para liberar memoria de forma segura, primero eliminados la memoria asignada a la cadena comand
        free(p); //Ahora si que si, liberamos el nodo al completo
        p = next; //Y hacemos que avance al siguiente
    }

    *L = LNULL; //Como al final la lista apunta a un valor liberado, para evitar errores de memoria hacemos que vuelva a apuntar a NULL
}
