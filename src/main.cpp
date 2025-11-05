#include <stdio.h>

#include "list.h"
#include "paint.h"

int main(){

    List myList = {};

    listErrors err = CORRECT_LIST;

    err = initList( &myList );
    if( err != CORRECT_LIST ){
        printf("\nInit null ptr\n");
        return 0;
    }

    int position = 100;                 // initialization

    position = listInsert( &myList, 10, 0 );
    printf("Position were was inserted = %d\n", position );
    position = listInsert( &myList, 20, 1 );
    printf("Position were was inserted = %d\n", position );
    position = listInsert( &myList, 30, 2 );
    printf("Position were was inserted = %d\n", position );
    position = listInsert( &myList, 40, 3 );
    printf("Position were was inserted = %d\n", position );
    position = listInsert( &myList, 50, 4 );
    //listDelete( &myList, 4 );
    printf("Position were was inserted = %d\n", position );
    position = listInsert( &myList, 60, position );
    printf("Position were was inserted = %d\n", position );
    position = listInsert( &myList, 35, 3 );
    printf("Position were was inserted = %d\n", position );
    position = listInsert( &myList, 36, position );
    printf("Position were was inserted = %d\n", position );
    position = listInsert( &myList, 37, position );
    printf("Position were was inserted = %d\n", position );
    position = listInsert( &myList, 38, position );
    printf("Position were was inserted = %d\n", position );
    position = listInsert( &myList, 39.5, position );
    printf("Position were was inserted = %d\n", position );
    //position = listInsert( &myList, 120, 5 );


    printList( &myList );

    dumpList( &myList );

    int indexNumberThatNeedFind = findElement( &myList, 35 );
    colorPrintf( NOMODE, YELLOW, "Index of element that you want: %d\n", indexNumberThatNeedFind );

    destroyList( &myList );

    return 0;
}


