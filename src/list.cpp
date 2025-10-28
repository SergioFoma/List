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

    myList->next[ 0 ] = 0;
    for( size_t nextIndex = 1; nextIndex < startSizeForArray - 1; nextIndex++ ){
        myList->next[ nextIndex ] = nextIndex + 1;
    }
    myList->next[ startSizeForArray - 1 ] = 0;

    myList->prev[ 0 ] = 0;
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

    // ARRAY PREV
    if( indexToPush >= myList->tailIndex && indexToPush != 0 ){
        myList->prev[ freeIndexNow ] = myList->tailIndex;
        //myList->prev[ myList->prev ] = freeIndexNow;
    }
    else if( freeIndexNow != 1 && indexToPush == 0 ){
        myList->prev[ freeIndexNow ] = myList->prev[ myList->headIndex ];
        printf("\n__________head Index_______ %lu\n", myList->headIndex );
        myList->prev[ myList->headIndex ] = freeIndexNow;
    }
    else if( freeIndexNow == 1 && indexToPush == 0 ){
        myList->prev[ freeIndexNow ] = indexToPush;
    }
    else{
        myList->prev[ freeIndexNow ] = indexToPush;
        myList->prev[ indexToPush + 1 ] = freeIndexNow;
    }

    // ARRAY NEXT
    myList->next[ freeIndexNow ] = myList->next[ indexToPush ];
    if( indexToPush != 0 ){
        myList->next[ indexToPush ] = freeIndexNow;
    }
    else if( freeIndexNow != 1 ){
        myList->next[ freeIndexNow ] = myList->headIndex;
        myList->headIndex = freeIndexNow;
    }

    if( myList->tailIndex <= indexToPush ){
        myList->tailIndex = freeIndexNow;
    }
    printf("\nindexTOPush = %lu\n", indexToPush );
    printList( myList );
    return freeIndexNow;
}

void printList( List* myList ){

    printf("\nheadIndex = %lu\n", myList->headIndex);
    printf("tailIndex = %lu\n", myList->tailIndex );
    for( size_t indexList = myList->headIndex; indexList != 0; indexList = myList->next[ indexList ] ){
        printf( "[%lu] = %lg ", indexList, myList->data[ indexList ] );
    }
    printf("\n");
    for( size_t indexNext = 1; indexNext < startSizeForArray; indexNext++ ){
        printf( "[%lu] = %d ", indexNext, myList->next[ indexNext ] );
    }
    printf("\n");
    for( size_t indexPrev = 1; indexPrev < startSizeForArray; indexPrev++ ){
        printf( "[%lu] = %d ", indexPrev, myList->prev[ indexPrev ] );
    }
    printf("\n");
    /*for( size_t indexList = myList->tailIndex; indexList != 0; indexList = myList->prev[ indexList ] ){
        printf( "[%lu] = %lg ", indexList, myList->data[ indexList ] );
    }*/
}
