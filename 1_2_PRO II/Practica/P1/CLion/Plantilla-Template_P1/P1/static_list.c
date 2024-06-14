/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 1
 * AUTHOR 1: ***************************** LOGIN 1: **********
 * AUTHOR 2: ***************************** LOGIN 2: **********
 * GROUP: *.*
 * DATE: ** / ** / **
 */

#include "static_list.h"


void createEmptyList(tList* L){
    L -> lastposition = LNULL;
}
bool isEmptyList(tList L){
    if (L.lastposition==LNULL)return true;
    else return false;
}
bool insertItem(tItemL d, tPosL p, tList* L){
    tPosL q;

    if (L ->lastposition == MAX-1) {//si la lista está llena, no se puede añadir elementos
        return false;
    }
    else{
        L ->lastposition++;//le sumo uno a la última posición porque añado un elemento a la lista
        if(p==LNULL) {
            L->content[L->lastposition] = d;//meto el elemento en la última posición
        }
        else{
            for(q = L -> lastposition; q > p; q--){
                L->content[q] = L ->content[q-1];//muevo los elementos de la lista un lugar para hacer hueco
            }
            L->content[p] = d;//finalmente asigno el elemento a la posicion
        }
        return true;
    }
}
tPosL first(tList L){
    return 0;
}
tPosL last(tList L){
    return L.lastposition;
}
tPosL next(tPosL p, tList L){
    if (p==L.lastposition)
        return LNULL;
    else
        return ++p;
}
tPosL previous(tPosL p, tList L){
    if (p==0)
        return LNULL;
    else
        return --p;
}
void deleteAtPosition(tPosL p, tList* L){
    tPosL q;

    for (q=p; q<L->lastposition;q++){
        L -> content[q]=L->content[q+1];
    }
    L -> lastposition--;
// En lenguaje C q=--p es que a q se le asigna el anterior y q=p-- se le asigna el numero que vale p
}
tItemL getItem(tPosL p, tList L){
    return L.content[p];
}
void updateItem(tItemL d, tPosL p, tList* L){
    //asignamos el elemento a la posición que nos da el usuario
    L ->content[p] = d;//p tiene que ser una posición válida, pero no efectuamos ningún tipo de control
}

tPosL findItem(tParticipantName d, tList L){
    tPosL p;
    if(L.lastposition == LNULL)
        return LNULL;//lista vacia, devuelve LNULL
    else{
        for(p=0; (p<L.lastposition) && (L.content[p].participantName != d); p++){
            if(L.content[p].participantName == d)
                return p;
            else
                return LNULL;
        }
    }
}
/* Write your code here... */

