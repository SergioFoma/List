#include <stdio.h>
#include <stdlib.h>

#include "list.h"

listErrors initList( List* myList ){
    if( myList == NULL ){
        return LIST_NULL_PTR;
    }

    myList->data = (double*)calloc( startSizeForArray, sizeof( double ) );
    CHECK_PTR( myList->data, DATA_INITIALIZATION_ERR );

    myList->next = (int*)calloc( startSizeForArray, sizeof( int ) );
    CHECK_PTR( myList->next, NEXT_INITIALIZATION_ERR );

    myList->prev = (int*)calloc( startSizeForArray, sizeof( int ) );
    CHECK_PTR( myList->next, PREV_INITIALIZATION_ERR );

    myList->freeIndex = 1;
    myList->headIndex = 1;
    myList->tailIndex = 0;
    myList->data[0] = canary;
    myList->data[ startSizeForArray - 1 ] = canary;

    for( size_t nextIndex = 1; nextIndex < startSizeForArray - 1; nextIndex++ ){
        myList->next[ nextIndex ] = nextIndex + 1;
    }
    myList->next[ startSizeForArray - 1 ] = 0;

    for( size_t prevIndex = 1; prevIndex < startSizeForArray; prevIndex++ ){
        myList->prev[ prevIndex ] = -1;
    }
    return CORRECT;
}

void destroyList( List* myList ){
    free( myList->data );
    free( myList->next );
    free( myList->prev );

    myList->freeIndex = 0;
    myList->headIndex = 0;
    myList->tailIndex = 0;

    myList->data = NULL;
    myList->next = NULL;
    myList->prev = NULL;
}

size_t listInsert( List* myList, double number , size_t indexToPush ){
    CHECK_PTR( myList, 0 );

    size_t freeIndexNow = myList->freeIndex ;
    myList->freeIndex = myList->next[ freeIndexNow ];

    myList->data[ freeIndexNow ] = number;

    myList->next[ freeIndexNow ] = myList->next[ indexToPush ];
    if( indexToPush != 0 ){
        myList->next[ indexToPush ] = freeIndexNow;
    }
    else if( freeIndexNow != 1 ){
        myList->next[ freeIndexNow ] = myList->headIndex;
        myList->headIndex = freeIndexNow;
    }


    /*printf("\nindexTOPush = %lu\n", indexToPush );
    printList( myList );*/
    return freeIndexNow;
}

void printList( List* myList ){

    printf("\nheadIndex = %lu\n", myList->headIndex);
    for( size_t indexList = myList->headIndex; indexList != 0; indexList = myList->next[ indexList ] ){
        printf("[%ld] = %lg ", indexList, myList->data[ indexList ] );
    }
    printf("\n");
    for(size_t indexNext = 1; indexNext < startSizeForArray; indexNext++ ){
        printf("[%ld] = %d ", indexNext, myList->next[ indexNext ] );
    }
    printf("\n");
}
