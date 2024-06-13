
#ifndef PARTICIPANT_LIST_H
#define PARTICIPANT_LIST_H

#include "types.h"

/* Write your code here... */

#include <stdbool.h>
#define NULLP NULL

typedef struct tItemP {
    tParticipantName participantName;
    tNumVotes numVotes;
    tEUParticipant EUParticipant;
} tItemP;
typedef struct tNode* tPosP;
struct tNode{
    tItemP data;
    tPosP next;
};
typedef tPosP tListP;

/*Prototipos de funciones*/

void createEmptyListP(tListP* L);//coloco el nombre de la operación y la implemento en el .c
bool insertItemP(tItemP d, tListP* L);
void updateItemP(tItemP d,tPosP p, tListP* L);
tPosP firstP(tListP L);
tPosP lastP(tListP L);
tPosP previousP(tPosP p, tListP L);
tPosP nextP(tPosP p, tListP L);
bool isEmptyListP(tListP L);
tItemP getItemP(tPosP p, tListP L);
tPosP findItemP(tParticipantName d, tListP L); //Duda si es como la PD1 o apuntes
void deleteListP(tListP* L);
void deleteAtPositionP(tPosP p, tListP* L);
bool copyListP(tListP L, tListP* M);

#endif
