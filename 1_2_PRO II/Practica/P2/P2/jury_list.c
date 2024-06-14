
#include "jury_list.h"
#include "participant_list.h"
#include <string.h>

void createEmptyListJ(tListJ *L){
    //(*L).lastPos = LNULL//es lo mismo
    L -> lastPos = NULLJ;//marco que la lista está vacía
}


bool insertItemJ(tItemJ d, tListJ *L) {
    tPosJ i;

    if (L->lastPos == MAX - 1)
        return false;
    else {
        if ((isEmptyListJ(*L)) || (strcmp(d.juryName, L->data[L->lastPos].juryName)>0)) {
            L->lastPos++;
            L->data[L->lastPos] = d;
        } else {
            L->lastPos++;
            for (i = L->lastPos; i > 0 && (strcmp(d.juryName, L->data[i - 1].juryName)<0); i--)
                L->data[i] = L->data[i - 1];
            L->data[i] = d;
        }
    }
    return true;
}

void updateItemJ(tItemJ d, tPosJ p, tListJ *L){
    //asignamos el elemento a la posición que nos da el usuario
    L ->data[p] = d;//p tiene que ser una posición válida, pero no efectuamos ningún tipo de control
}

bool copyListJ(tListJ L, tListJ *M){
    tPosJ p;
    createEmptyListJ(M);
    if(!isEmptyListJ(L)){
        for(p = 0; p<L.lastPos;p++){
            M->data[p] = L.data[p];
        }
        M->lastPos = L.lastPos;
    }
    return true;
}

tPosJ firstJ(tListJ L){
    return 0;
}

tPosJ lastJ(tListJ L){
    return L.lastPos;
}

tPosJ previousJ(tPosJ p, tListJ L){
    if (p==0)
        return NULLJ;
    else
        return --p;
}

tPosJ nextJ(tPosJ p, tListJ L){
    if (p==L.lastPos)
        return NULLJ;
    else
        return ++p;
}

bool isEmptyListJ(tListJ L){
    return L.lastPos == NULLJ;
}
tItemJ getItemJ(tPosJ p, tListJ L){
    return L.data[p];
}

tPosJ findItemJ(tJuryName d, tListJ L){
    tPosJ p;
    if(L.lastPos == NULLJ)
        return NULLJ;
    else if((strcmp(d, L.data[0].juryName)<0) || (strcmp(d, L.data[L.lastPos].juryName)>0)) //Si se cumple que es mayor o menor que lastPos devuelve null
        return NULLJ;
    else {
        for(p=0; (p<L.lastPos) && (strcmp(L.data[p].juryName, d)<0); p++);
        if(strcmp(L.data[p].juryName, d)==0)
            return p;
        else
            return NULLJ;
    }
}

void deleteListJ(tListJ* L){
    /*while(isEmptyList(*L) != true)
        deleteAtPosition(last(*L), L);
        */
    L ->lastPos = NULLJ;
}

void deleteAtPositionJ(tPosJ p, tListJ* L){
    tPosJ q;

    for (q=p; q<L->lastPos;q++){
        L -> data[q]=L->data[q+1];
    }
    L -> lastPos--;
// En lenguaje C q=--p es que a q se le asigna el anterior y q=p-- se le asigna el numero que vale p
}

/* Write your code here... */
