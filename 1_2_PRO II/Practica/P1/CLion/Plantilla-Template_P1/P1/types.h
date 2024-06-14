#ifndef PRO2_2023_P1_TYPES_H
#define PRO2_2023_P1_TYPES_H


#define NAME_LENGTH_LIMIT 25
#define MAX 45
#include <stdbool.h>
#include <string.h>

//#########################
typedef char tParticipantName[NAME_LENGTH_LIMIT];
typedef int tNumVotes;
typedef bool tEUParticipant;
typedef struct tItemL {
    tParticipantName participantName;
    tNumVotes numVotes;
    tEUParticipant EUParticipant;
} tItemL;
//############################

#define LNULL -1
typedef int tPosL;

typedef struct{
    tItemL content[MAX];
    tPosL lastposition;
}tList;



#endif //PRO2_2023_P1_TYPES_H
