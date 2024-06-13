
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"

#define MAX_BUFFER 255

#ifdef DYNAMIC_LIST
#include "dynamic_list.h"
#endif
#ifdef STATIC_LIST
#include "static_list.h"
#endif

void fNew( char *param1, char *param2, tList *list){
    tItemL item;
    strcpy(item.participantName,param1); //Inicializamos el nombre del participante de item
    item.numVotes=0; //Inicializamos los votos a 0
    if(strcmp(param2,"eu") == 0)
        item.EUParticipant=true;
    else
        item.EUParticipant=false; //comprobamos el parámetro "eu" y "non-eu"

    if(findItem(param1, *list) == LNULL && insertItem(item, LNULL, list)) {
        printf("* New: participant %s location %s\n", param1, param2);
        updateItem(item, findItem(param1, *list), list); //Finalmente se actualiza la lista
    }else { //Si no existe ningun participante con ese nombre y no hay error al meterlo en la lista, se notifica
        printf("+ Error: New not possible\n");
    }

}

void fVote(char* param1, tList *list, int* cN, int* cV){
    tPosL p = findItem(param1, *list);// Inicializamos un nodo en la posicion donde queremos meter el voto
    tItemL item;

    if (isEmptyList(*list) == true || p == LNULL) {
        printf("+ Error: Vote not possible. Participant %s not found. NULLVOTE\n", param1);
        (*cN)++; //Contador de votos nulos
        (*cV)++; //Contador de votos realizados
    } else { //En caso de que no encuentre al participante
        item = getItem(p, *list); //Ahora que sabemos que el elemento está en la lista, obtenemos los valores del item y hacemos las operaciones restantes
        item.numVotes++;
        (*cV)++; //Contador de votos hechos
        printf("* Vote: participant %s location %s numvotes %d\n",param1, item.EUParticipant?"eu":"non-eu", item.numVotes);

        updateItem(item, p, list); //Finalmente se actualiza la lista
    }
}

void fDisqualify(tList* list, char* param1, int* cN){
    tPosL p = findItem(param1, *list);//Al igual que en fVote, buscamos el nodo que queremos;
    tItemL item;

    if(isEmptyList(*list)==true){
        printf("+ Error: Disqualify not possible\n");
    } else{ //Si la lista esta vacia, hacemos que no haga ninguna operacion
        item = getItem(p, *list);//Obtenemos los datos del item que estamos tratando
        *cN += item.numVotes; //Y como los votos del pais a borrar existen, los pasamos a los votos en NULL
        printf("* Disqualify: participant %s location %s\n", param1, item.EUParticipant?"eu":"non-eu");
        deleteAtPosition(findItem(param1, *list), list);//Para ahorrar memoria, eliminamos el espacio que ocupaba anteriormente
    }
}

void fStats(tList *list, char* param1, int* cN, int* cV){
    tPosL p;
    tItemL item;

    if(isEmptyList(*list)==true){
        printf("Lista vacia\n"); //Primero comprueba si la lista está vacía
    }
    for(p = first(*list); p != LNULL; p = next(p, *list)){
        item= getItem(p, *list);

        printf("Participant %s location %s numvotes %d ",item.participantName,
               item.EUParticipant?"eu":"non-eu",
               item.numVotes);
        if (*cV == 0 || (*cV - *cN) == 0) { //Esto evita que se divida entre 0 cualquier numero, ya que devolverá NaN (Not a Number) en caso afirmativo
            printf("(%.2f%%)\n", (float) 0);
        } else
            printf("(%.2f%%)\n", (float) item.numVotes / (*cV - *cN) * 100);

        //En este bucle for, recorre el arreglo/punteros hasta no encontar nada dando su respetiva información
    }
    printf("Null votes %d\n", *cN); //Cantidad de votos nulos
    printf("Participation: %d votes from %d voters (%.2f%%)\n", *cV, atoi(param1),(float) *cV /atoi(param1) * 100);
}

void processCommand(tList *list, char *commandNumber, char command, char *param1, char *param2, int* cntNULL, int* cntVOTES) {

    printf("********************\n%s %c: ", commandNumber, command);

    switch (command) {
        case 'N': //acción New
            printf("participant %s location %s\n", param1, param2);//peticion

            fNew( param1, param2, list);
            break;

        case 'V': //acción Vote
            printf("participant %s \n", param1);

            fVote(param1, list, cntNULL, cntVOTES);
            break;

        case 'D': //acción Disqualify
            printf("participant %s \n", param1);

            fDisqualify( list, param1, cntNULL);
            break;

        case 'S': //acción Stats
            printf("totalvoters %s\n", param1);

            fStats( list, param1, cntNULL,cntVOTES);
            break;

        default: //En caso de que lea una letra no pensada para la ejecución
            printf("+ Error: Action %c does not exist\n", command);

            break;
    }
}

void readTasks(char *filename, tList *L) {

    //Añadimos dos variables de contador
    int cntNULL=0;//Para votos nulos
    int cntVOTES=0;//Para votos totales

    FILE *f = NULL;
    char *commandNumber, *command, *param1, *param2;
    const char delimiters[] = " \n\r";
    char buffer[MAX_BUFFER];

    f = fopen(filename, "r");

    if (f != NULL) {

        while (fgets(buffer, MAX_BUFFER, f)) {
            commandNumber = strtok(buffer, delimiters);
            command = strtok(NULL, delimiters);
            param1 = strtok(NULL, delimiters);
            param2 = strtok(NULL, delimiters);

            processCommand(L,commandNumber, command[0], param1, param2, &cntNULL, &cntVOTES);
        }

        fclose(f);

    } else {
        printf("Cannot open file %s.\n", filename);
    }
}


int main(int nargs, char **args) {
    tList L;//Lista con la que vamos a trabajar
    createEmptyList(&L);

    char *file_name = "new.txt";

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