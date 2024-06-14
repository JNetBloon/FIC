
#ifndef JURY_LIST_H
#define JURY_LIST_H

#include "types.h"
#include "participant_list.h"

#define MAX 25
#define NULLJ -1
typedef int tPosJ;

typedef struct tItemJ {
    tJuryName juryName;
    tNumVotes totalVoters;
    tNumVotes validVotes;
    tNumVotes nullVotes;
    tListP participantList;
} tItemJ;
typedef struct {
    tItemJ data[MAX];
    tPosJ lastPos;
}tListJ;

//FUNCIONES

void createEmptyListJ(tListJ* L);//coloco el nombre de la operación y la implemento en el .c
bool insertItemJ(tItemJ d, tListJ* L);
void updateItemJ(tItemJ d, tPosJ p, tListJ* L);
bool copyListJ(tListJ L, tListJ *M);
tPosJ firstJ(tListJ L);
tPosJ lastJ(tListJ L);
tPosJ previousJ(tPosJ p, tListJ L);
tPosJ nextJ(tPosJ p, tListJ L);
bool isEmptyListJ(tListJ L);
tItemJ getItemJ(tPosJ p, tListJ L);
tPosJ findItemJ(tJuryName d, tListJ L);
void deleteListJ(tListJ *L);
void deleteAtPositionJ(tPosJ p, tListJ* L);


/* Write your code here... */

#endif
