
#include "dynamic_list.h"
#include <stdlib.h>
#include <string.h>


void createEmptyList(tList* L){
    *L = LNULL;
}

bool createNode(tPosL* p){
    *p = malloc(sizeof(struct tNode));
    return *p != LNULL;
}

bool insertItem(tItemL d, tPosL p, tList* L){
    tPosL q, r;

    if (!createNode(&q))
        return false;
    else {
        q -> content = d;//inicializa q
        q -> next = LNULL;

        if(*L == LNULL) //Si la lista esta vacia
            *L = q;
        else if(p == LNULL){ //Si lo quieres poner en una posicion no existente
            for(r= *L; r->next != LNULL; r = r->next);
            r->next = q;
        } else if (p == *L){//Si p y L son LNULL lo asigna en el next
            q -> next = *L;
            *L = q;
        } else{//q = p;
            q->content = p->content;
            p->content = d;
            q->next = p->next;
            p->next = q;
        }
        return true;
    }

}

void updateItem(tItemL d, tPosL p, tList* L){
    p->content = d;
}

tPosL findItem(tParticipantName d, tList L){
    tList p;

    for(p=L;(p != LNULL) && strcmp(p->content.participantName, d)!=0;p = p->next);
    return p;
}

tPosL first(tList L){
    return L;
}

tPosL last(tList L){
    tPosL p;

    for(p = L; p->next != LNULL; p= p->next);
    return p;
}
tPosL previous(tPosL p, tList L){
    tPosL q;

    if (p == L)
        return LNULL;
    else{
        for(q = L; q->next != p; q = q->next);
        return q;
    }
}
tPosL next(tPosL p, tList L){
    return p->next;
}
bool isEmptyList(tList L){
    return L == LNULL;
}

tItemL getItem(tPosL p, tList L){
    return p->content;
}

void deleteList(tList* L){
    tPosL p;

    while(*L != NULL){
        p = *L;
        *L = (*L)->next;
        free(p);
    }
}

void deleteAtPosition(tPosL p, tList* L){
    tPosL q;

    if(p == *L)
        *L = (*L)->next;
    else if (p->next == NULL){
        for(q = *L; q->next != p; q= q -> next);
        q->next = LNULL;
    } else {
        q = p->next;
        p->content = q->content;
        p->next = q->next;
        p = q;
    }
    free(p);
    p = LNULL;
}

/* Write your code here... */