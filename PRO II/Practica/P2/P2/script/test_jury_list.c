#include <stdio.h>
#include <string.h>

#include "jury_list.h"

void print_list(tListJ list) {
    tPosJ pos;
    tItemJ item;

    printf("(");
    if (!isEmptyListJ(list)) {
        pos = firstJ(list);
        while (pos != NULLJ) {
            item = getItemJ(pos, list);
            printf(" %s totalVoters %d", item.juryName, item.totalVoters);
            pos = nextJ(pos, list);
        }
    }
    printf(")\n");
}

int main() {
    tListJ list;
    tPosJ pos;
    tItemJ item1, item2;


    /* create */
    
    createEmptyListJ(&list);
    print_list(list);

    /* insert */
    strcpy(item1.juryName, "jury3");
    item1.totalVoters = 10;

    insertItemJ(item1, &list);
    print_list(list);

    strcpy(item1.juryName, "jury1");
    insertItemJ(item1, &list);
    print_list(list);

    strcpy(item1.juryName, "jury5");
    insertItemJ(item1, &list);
    print_list(list);

    strcpy(item1.juryName, "jury2");
    insertItemJ(item1,&list);
    print_list(list);

    strcpy(item1.juryName, "jury4");
    insertItemJ(item1, &list);
    print_list(list);


    /* find */
    pos = findItemJ("jury33", list);
    if (pos == NULLJ) {
        printf("jury33 Not found\n");
    }


    pos = findItemJ("jury3", list);
    item2 = getItemJ(pos, list);
    printf("%s\n", item2.juryName);


    /* update */
    item2 = getItemJ(pos, list);
    item2.totalVoters = 15;
    updateItemJ(item2,pos,&list);
    printf("%s\n", item2.juryName);
    print_list(list);

    /* remove */
    deleteAtPositionJ(nextJ(firstJ(list),list), &list);
    print_list(list);

    deleteAtPositionJ(previousJ(lastJ(list),list), &list);
    print_list(list);

    deleteAtPositionJ(firstJ(list),&list);
    print_list(list);

    deleteAtPositionJ(lastJ(list),&list);
    print_list(list);

    deleteAtPositionJ(firstJ(list),&list);
    print_list(list);

    insertItemJ(item1, &list);
    print_list(list);

}
