
#include "static_list.h"
#include <string.h>

void createEmptyList(tList* L){
    L -> lastposition = LNULL;
}
bool isEmptyList(tList L){
    if (L.lastposition==LNULL)return true;
    else return false;
}
bool insertItem(tItemL d, tPosL p, tList* L){
    tPosL q;

    if (L ->lastposition == MAX-1) {
        return false;
    } else{
        L ->lastposition++;
        if(p==LNULL) {
            L->content[L->lastposition] = d;
        }
        else{
            for(q = L -> lastposition; q > p; q--){
                L->content[q] = L ->content[q-1];
            }
            L->content[p] = d;
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
    if (p==L.lastposition){
        return LNULL;
    } else {
        return ++p;
    }
}
tPosL previous(tPosL p, tList L){
    if (p==0){
        return LNULL;
    } else{
        return --p;
    }
}
void deleteAtPosition(tPosL p, tList* L){
    tPosL q;

    for (q=p; q<L->lastposition;q++){
        L -> content[q]=L->content[q+1];
    }
    L -> lastposition--;
}
tItemL getItem(tPosL p, tList L){
    return L.content[p];
}
void updateItem(tItemL d, tPosL p, tList* L){
    L ->content[p] = d;
}

tPosL findItem(tParticipantName d, tList L){
    tPosL p;
    if(L.lastposition == LNULL){
        return LNULL;
    } else{
        for(p=0; (p<L.lastposition) && strcmp(L.content[p].participantName,d)!=0;p++);

        if(strcmp(L.content[p].participantName,d)==0){
            return p;
        }
        else {
            return LNULL;
        }
    }
}

