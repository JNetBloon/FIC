
#include "participant_list.h"
#include <stdlib.h>
#include <string.h>

void createEmptyListP(tListP* L){
    *L = NULLP;
}

bool createNodeP(tPosP* p){
    *p = malloc(sizeof(struct tNode));
    return *p != NULLP;
}

tPosP findPositionP(tListP L, tItemP d){
    tPosP p;
    p = L;
    while((p->next!=NULLP) && (strcmp(p->next->data.participantName, d.participantName)<0))
        p = p ->next;
    return p;
}

bool insertItemP(tItemP d, tListP* L){
    tPosP p, q;
    if (!createNodeP(&q))
        return false;
    else {
        q->data = d;
        q->next = NULLP;

        if(*L == NULLP)
            *L = q;
        else if(strcmp(d.participantName, (*L)->data.participantName)<0){
            q->next = *L;
            *L = q;
        } else{
            p = findPositionP(*L, d);
            q->next = p->next;
            p->next = q;
        }
        return true;
    }
    //return true;
}

void updateItemP(tItemP d, tPosP p, tListP* L){
    p->data = d;
}

tPosP findItemP(tParticipantName d, tListP L){
    tPosP p;

    for(p=L;(p != NULLP) && strcmp(p->data.participantName, d)<0;p = p->next);
    if(p != NULLP && (strcmp(p->data.participantName, d)==0)){ //Si son iguales devuelve pos
        return p;
    } else{
        return NULLP;
    }
}

tPosP firstP(tListP L){
    return L;
}

tPosP lastP(tListP L){
    tPosP p;

    for(p = L; p->next != NULLP; p= p->next);
    return p;
}
tPosP previousP(tPosP p, tListP L){
    tPosP q;

    if (p == L)
        return NULLP;
    else{
        for(q = L; q->next != p; q = q->next);
        return q;
    }
}
tPosP nextP(tPosP p, tListP L){
    return p->next;
}
bool isEmptyListP(tListP L){
    return L == NULLP;
}

tItemP getItemP(tPosP p, tListP L){
    return p->data;
}

void deleteListP(tListP* L){
    tPosP p;

    while(*L != NULL){
        p = *L;
        *L = (*L)->next;
        free(p);
    }
}

void deleteAtPositionP(tPosP p, tListP* L){
    tPosP q;

    if(p == *L)
        *L = (*L)->next;
    else if (p->next == NULL){
        for(q = *L; q->next != p; q= q -> next);
        q->next = NULLP;
    } else {
        q = p->next;
        p->data = q->data;
        p->next = q->next;
        p = q;
    }
    free(p);
    p = NULLP;
}