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

    listInsert( &myList, 10, 0 );
    listInsert( &myList, 20, 1 );
    listInsert( &myList, 30, 2 );
    listInsert( &myList, 40, 3 );
    listInsert( &myList, 50, 4 );
    listInsert( &myList, 101, 0 );
    listInsert( &myList, 60, 5 );
    listInsert( &myList, 35, 3 );


    printList( &myList );

    destroyList( &myList );

    return 0;
}


