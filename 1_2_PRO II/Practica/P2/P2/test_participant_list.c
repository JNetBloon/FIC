#include <stdio.h>
#include <string.h>

#include "participant_list.h"

void print_list(tListP list) {
    tPosP pos;
    tItemP item;

    printf("(");
    if (!isEmptyListP(list)) {
        pos = firstP(list);
        while (pos != NULLP) {
            item = getItemP(pos, list);
            printf(" %s numvotes %d", item.participantName, item.numVotes);
            pos = nextP(pos, list);
        }
    }
    printf(")\n");
}

int main() {
    tListP list;
    tPosP pos;
    tItemP item1, item2;


    /* create */
    
    createEmptyListP(&list);
    print_list(list);

    /* insert */
    strcpy(item1.participantName, "country3");
    item1.numVotes = 0;

    insertItemP(item1, &list);
    print_list(list);

    strcpy(item1.participantName, "country1");
    insertItemP(item1, &list);
    print_list(list);

    strcpy(item1.participantName, "country5");
    insertItemP(item1, &list);
    print_list(list);

    strcpy(item1.participantName, "country2");
    insertItemP(item1,&list);
    print_list(list);

    strcpy(item1.participantName, "country4");
    insertItemP(item1, &list);
    print_list(list);


    /* find */
    pos = findItemP("country33", list);
    if (pos == NULLP) {
        printf("country33 Not found\n");
    }


    pos = findItemP("country3", list);
    item2 = getItemP(pos, list);
    printf("%s\n", item2.participantName);


    /* update */
    item2 = getItemP(pos, list);
    item2.numVotes = 2;
    updateItemP(item2,pos,&list);
    printf("%s\n", item2.participantName);
    print_list(list);

    /* remove */
    deleteAtPositionP(nextP(firstP(list),list), &list);
    print_list(list);

    deleteAtPositionP(previousP(lastP(list),list), &list);
    print_list(list);

    deleteAtPositionP(firstP(list),&list);
    print_list(list);

    deleteAtPositionP(lastP(list),&list);
    print_list(list);

    deleteAtPositionP(firstP(list),&list);
    print_list(list);

    insertItemP(item1, &list);
    print_list(list);

}
