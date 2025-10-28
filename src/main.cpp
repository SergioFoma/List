#include <stdio.h>

#include "list.h"

int main(){

    List myList = {};

    listErrors err = CORRECT;

    initList( &myList );

    /*listInsert( &myList, 50, 3 );

    listInsert( &myList, 60, 4 );

    listInsert( &myList, 40, 3 );

    listInsert( &myList, 30, 2 );

    listInsert( &myList, 20, 1 );

    listInsert( &myList, 10, 0 );*/

    size_t position = 100;

    position = listInsert( &myList, 10, 0 );
    printf("Position were was inserted = %lu\n", position );
    position = listInsert( &myList, 20, 1 );
    printf("Position were was inserted = %lu\n", position );
    position = listInsert( &myList, 30, 2 );
    printf("Position were was inserted = %lu\n", position );
    position = listInsert( &myList, 40, 3 );
    printf("Position were was inserted = %lu\n", position );
    position = listInsert( &myList, 50, 4 );
    printf("Position were was inserted = %lu\n", position );
    position = listInsert( &myList, 101, 0 );
    printf("Position were was inserted = %lu\n", position );
    position = listInsert( &myList, 60, 5 );
    printf("Position were was inserted = %lu\n", position );
    position = listInsert( &myList, 35, 3 );
    printf("Position were was inserted = %lu\n", position );


    printList( &myList );

    destroyList( &myList );

    return 0;
}


