
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "jury_list.h"

#define MAX_BUFFER 255

void create(char *jury, char* totalvoters, tListJ *listJ){
    tItemJ itemJ; tPosJ pJ;

    int tvote=atoi(totalvoters);
    printf("jury %s totalvoters %d\n",jury, tvote);

    itemJ.totalVoters=tvote;
    itemJ.validVotes=0;
    itemJ.nullVotes=0;
    strcpy(itemJ.juryName,jury);

    pJ=findItemJ(jury, *listJ);
    if((pJ==NULLJ) && (insertItemJ(itemJ, listJ))){
        tListP listP;
        createEmptyListP(&listP);
        listJ->data[findItemJ(jury, *listJ)].participantList = listP;
        printf("* Create: jury %s totalvoters %d\n", jury, tvote);
    } else{
        printf("+ Error: Create not possible\n");
    }
}

void new(char* jury, char* participant, char* loc, tListJ *listJ){
    tItemJ itemJ; tPosJ pJ; tItemP itemP;

    printf("jury %s participant %s location %s\n", jury, participant, loc);

    pJ= findItemJ(jury, *listJ);

    if(pJ==NULLJ){
        printf("+ Error: New not possible\n");
    } else {
        itemJ = getItemJ(pJ, *listJ);
        if(findItemP(participant, itemJ.participantList)==NULLP){
            strcpy(itemP.participantName,participant);
            if(strcmp(loc, "eu")==0){
                itemP.EUParticipant=true;
            } else{
                itemP.EUParticipant=false;
            }
            itemP.numVotes=0;

            if(insertItemP(itemP, &itemJ.participantList)==true){
                updateItemJ(itemJ, pJ, listJ);
                updateItemP(itemP, findItemP(participant, itemJ.participantList), &itemJ.participantList);
                printf("* New: jury %s participant %s location %s\n", jury, participant, loc);
            } else{
                printf("+ Error: New not possible\n");
            }
        } else{
            printf("+ Error: New not possible\n");
        }
    }
}

void vote(char* jury, char* participant, tListJ *listJ){
    tItemJ itemJ; tItemP itemP; tPosJ pJ; tPosP pP;
    printf("jury %s participant %s\n",jury, participant);
    pJ = findItemJ(jury, *listJ);

    if(pJ!=NULLJ){
        itemJ= getItemJ(pJ, *listJ);
        pP = findItemP(participant, itemJ.participantList);
        if(pP!=NULLP){
            itemP = getItemP(pP, itemJ.participantList);
            itemJ.validVotes++;
            itemP.numVotes++;
            updateItemP(itemP, findItemP(participant, itemJ.participantList), &itemJ.participantList);

            printf("* Vote: jury %s participant %s location %s numvotes %d\n", jury, participant
                                                                                    ,itemP.EUParticipant?"eu":"non-eu"
                                                                                    ,itemP.numVotes);
        } else {
            printf("+ Error: Vote not possible. Participant %s not found in jury %s. NULLVOTE\n", participant, jury);
            itemJ.validVotes++;
            itemJ.nullVotes++;
        }

        updateItemJ(itemJ, pJ, listJ);
    } else{
        printf("+ Error: Vote not possible\n");
    }
}

void stats(char *jury, char* tvote, tListJ *listJ){
    tItemJ itemJ; tItemP itemP; tPosJ pJ; tPosP pP;

    printf("\n");//WARNING AQUI PUEDE FALTAR UN ESPACIO

    if(isEmptyListJ(*listJ)==true){
        printf("+ Error: Stats not possible\n");
    } else{
        for(pJ = firstJ(*listJ); pJ!=NULLJ; pJ= nextJ(pJ, *listJ)){
            itemJ = getItemJ(pJ, *listJ);
            printf("Jury %s\n", itemJ.juryName);

            //itemP = itemJ.participantList->data;
            //*listP = listJ->data[findItemJ(param1, *listJ)].participantList;
            //*listP = listJ->data[pJ].participantList;

            if(isEmptyListP(itemJ.participantList) == true){//Comprueba que haya participantes
                printf("No participants\n");
            } else {
                for (pP = firstP(itemJ.participantList); pP != NULLP; pP = nextP(pP, itemJ.participantList)) {//Por cada participante, saca estadísticas
                    itemP = getItemP(pP, itemJ.participantList);
                    itemJ = getItemJ(pJ, *listJ);

                    float PerVotes;
                    if(itemJ.validVotes==0){
                        PerVotes= (float) itemP.numVotes;
                    } else{
                        PerVotes = (float)(itemP.numVotes)/ (float) itemJ.validVotes;
                    }

                    printf("Participant %s location %s numvotes %d (%.2f%%)\n", itemP.participantName
                                                                                    , itemP.EUParticipant ? "eu":"non-eu"
                                                                                    , itemP.numVotes
                                                                                    , PerVotes*100);
                }
            }
            printf("Nullvotes %d\n", itemJ.nullVotes);
            float TotalVotes = (float) itemJ.validVotes/ (float) itemJ.totalVoters;
            printf("Participation: %d votes from %d voters (%.2f%%)\n", itemJ.validVotes, itemJ.totalVoters, TotalVotes*100);
            printf("\n");
        }
    }
}

void rmv(char *jury, char* tvote, tListJ *listJ) {
    tItemJ itemJ; tItemP itemP; tPosJ pJ; tPosP pP;

    printf("\n");//WARNING AQUI PUEDE FALTAR UN ESPACIO

    pJ= firstJ(*listJ);

    while(pJ != NULLJ) {
        itemJ = getItemJ(pJ, *listJ);
        if(itemJ.validVotes==0){
            printf("* Remove: jury %s\n", itemJ.juryName);
            deleteListP(&itemJ.participantList);
            deleteAtPositionJ(pJ, listJ);
            pJ = firstJ(*listJ);
        } else{
            pJ= nextJ(pJ, *listJ);
        }
    }
}

void disqualify(char* participant, tListJ *listJ) {
    tItemJ itemJ; tItemP itemP; tPosJ pJ; tPosP pP;

    printf(" participant %s\n", participant);//WARNING AQUI PUEDE FALTAR UN ESPACIO

    if(isEmptyListJ(*listJ)==true){
        printf("+ Error: Disqualify not possible\n");
    } else{
        for(pJ = firstJ(*listJ); pJ!=NULLJ; pJ= nextJ(pJ, *listJ)){
            itemJ = getItemJ(pJ, *listJ);
            pP = findItemP(participant, itemJ.participantList);
            printf("Jury %s\n", itemJ.juryName);

            if((isEmptyListP(itemJ.participantList) == false) && (pP != NULLP)) {//Comprueba que haya participantes
                printf("Participant %s disqualified\n\n", participant);
                itemJ.nullVotes = itemJ.nullVotes + itemJ.participantList->data.numVotes;
                deleteAtPositionP(pP, &itemJ.participantList);

                updateItemJ(itemJ, pJ, listJ);
            }

        }
    }
}

void winners(char *jury, char* tvote, tListJ *listJ) {
    tItemJ itemJ; tItemP itemP; tPosJ pJ; tPosP pP;
    tPosP EUWinnerP; tPosP NEUWinnerP;
    bool EUtie = false; bool NEUtie = false;

    printf("\n");//WARNING AQUI PUEDE FALTAR UN ESPACIO

    if(isEmptyListJ(*listJ)==true){
        printf("+ Error: Winners not possible\n");
    } else{
        for(pJ = firstJ(*listJ); pJ!=NULLJ; pJ= nextJ(pJ, *listJ)){
            itemJ = getItemJ(pJ, *listJ);
            printf("Jury %s\n", itemJ.juryName);

            EUWinnerP = NULLP; NEUWinnerP = NULLP;

            for(pP = firstP(itemJ.participantList); pP!=NULLP; pP = nextP(pP, itemJ.participantList)){
                if(pP->data.EUParticipant==true){
                    if(EUWinnerP==NULLP){
                        EUWinnerP = pP;
                    } else {
                        if(pP->data.numVotes > EUWinnerP->data.numVotes){
                            EUWinnerP = pP;
                            EUtie = false;
                        } else if (pP->data.numVotes == EUWinnerP->data.numVotes){
                            EUtie = true;
                        }
                    }
                } else {
                    if(NEUWinnerP==NULLP){
                        NEUWinnerP = pP;
                    } else {
                        if(pP->data.numVotes > NEUWinnerP->data.numVotes){
                            NEUWinnerP = pP;
                            NEUtie = false;
                        } else if (pP->data.numVotes == EUWinnerP->data.numVotes){
                            NEUtie = true;
                        }
                    }
                }
            }

            if(EUWinnerP==NULLP || EUtie==true){
                printf("Location eu: No winner\n");
            } else {
                printf("Location eu: Participant %s numvotes %d\n", EUWinnerP->data.participantName, EUWinnerP->data.numVotes);
            }
            if(NEUWinnerP==NULLP || NEUtie==true){
                printf("Location non-eu: No winner\n");
            } else {
                printf("Location non-eu: Participant %s numvotes %d\n", NEUWinnerP->data.participantName, NEUWinnerP->data.numVotes);
            }
            printf("\n");
        }
    }

}

void processCommand(tListJ *listJ, char *commandNumber, char command, char *param1, char *param2, char *param3) {

    tItemJ itemJ; tItemP itemP;
    tPosJ pJ; tPosP pP;
    printf("********************\n%s %c:",commandNumber,command);

    switch (command) {
        case 'C':
            create(param1, param2, listJ);
            break;
        case 'N':
            new(param1, param2, param3, listJ);
            break;
        case 'V':
            vote(param1, param2, listJ);
            break;
        case 'D':
            disqualify(param1, listJ);
            break;
        case 'S':
            stats(param1, param2, listJ);
            break;
        case 'R':
            rmv(param1, param2, listJ); //Esto lo que hace es eliminar todos los jurados con 0 votos validos (validnotes)
            //Existe la funcion remove, asi que se llamara rmv para que no haya problemas
            break;
        case 'W':
            winners(param1, param2, listJ);
            break;
        default:
            break;
    }
}

void readTasks(char *filename, tListJ *L) {

    FILE *f = NULL;
    char *commandNumber, *command, *param1, *param2, *param3;
    const char delimiters[] = " \n\r";
    char buffer[MAX_BUFFER];

    f = fopen(filename, "r");

    if (f != NULL) {

        while (fgets(buffer, MAX_BUFFER, f)) {
            commandNumber = strtok(buffer, delimiters);
            command = strtok(NULL, delimiters);
            param1 = strtok(NULL, delimiters);
            param2 = strtok(NULL, delimiters);
            param3 = strtok(NULL, delimiters);

            processCommand(L, commandNumber, command[0], param1, param2, param3);
        }

        fclose(f);

    } else {
        printf("Cannot open file %s.\n", filename);
    }
}


int main(int nargs, char **args) {
    tListJ L;//
    createEmptyListJ(&L);//

    char *file_name = "create.txt";

    if (nargs > 1) {
        file_name = args[1];
    } else {
        #ifdef INPUT_FILE
        file_name = INPUT_FILE;
        #endif
    }

    readTasks(file_name, &L);

    return 0;
}
