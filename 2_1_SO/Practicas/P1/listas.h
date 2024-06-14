
#ifndef SHELLTEST_LISTAS_H
#define SHELLTEST_LISTAS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define LNULL NULL

typedef char *tData;
typedef struct tNode* tPosL;

struct tNode{
    tData comand;
    tPosL next;
};

typedef tPosL tList;

void createList(tList* L); //Crear lista vacia
bool isEmptyList(tList L); //Verificar si la lista esta vacia o no
bool insertElement(tData d, tList* L); //Insercion de comando
tData getElement(char *charpos, tList L); //Obtenemos el nombre del comando en la posicion N
void PrintList(tList L); //Imprime la lista al completo
void PrintNList(int N, tList L); //Imprime los primeros N numeros del historial
void deleteList(tList *L); //Borramos el contenido de la lista

#endif //SHELLTEST_LISTAS_H
