#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "checkListError.h"
#include "paint.h"

listErrors initList( List* myList ){
    CHECK_PTR( myList, LIST_NULL_PTR );

    myList->data = (listValue*)calloc( startSizeForArray, sizeof( listValue ) );
    CHECK_PTR( myList->data, DATA_INITIALIZATION_ERR );

    myList->next = (int*)calloc( startSizeForArray, sizeof( int ) );
    CHECK_PTR( myList->next, NEXT_INITIALIZATION_ERR );

    myList->prev = (int*)calloc( startSizeForArray, sizeof( int ) );
    CHECK_PTR( myList->next, PREV_INITIALIZATION_ERR );

    myList->freeIndex = 1;
    myList->sizeOfList = startSizeForArray;
    myList->countOfElement = 1;
    myList->data[0] = canary;
    myList->data[ startSizeForArray - 1 ] = canary;

    myList->next[ headPosition ] = 0;
    for( size_t nextIndex = 1; nextIndex < startSizeForArray - 1; nextIndex++ ){
        myList->next[ nextIndex ] = (int)nextIndex + 1;
    }
    myList->next[ startSizeForArray - 1 ] = 0;

    myList->prev[ tailPosition ] = 0;
    for( size_t prevIndex = 1; prevIndex < startSizeForArray; prevIndex++ ){
        myList->prev[ prevIndex ] = -1;
    }
    return CORRECT_LIST;
}

void destroyList( List* myList ){
    CHECK_PTR( myList, (void)0 );

    free( myList->data );
    free( myList->next );
    free( myList->prev );

    myList->freeIndex = 0;
    myList->sizeOfList = 0;
    myList->countOfElement = 0;

    myList->data = NULL;
    myList->next = NULL;
    myList->prev = NULL;
}

int listInsert( List* myList, listValue number , size_t indexToPush ){
    CHECK_LIST( myList, -1 );

    listErrors statusOfMemoryRealloc = reallocateList( myList, indexToPush );
    if( statusOfMemoryRealloc != CORRECT_LIST ){
        return -1;
    }
    else if( indexToPush + 1 > myList->freeIndex ){
        colorPrintf( NOMODE, RED, "\n\nYou want to insert element on Position %lu, but position %lu is free\n\n", indexToPush + 1, myList->freeIndex );
        return -1;
    }

    size_t freeIndexNow = myList->freeIndex ;
    myList->freeIndex = (size_t)myList->next[ freeIndexNow ];
    myList->data[ freeIndexNow ] = number;

    // ARRAY NEXT
    myList->next[ freeIndexNow ] = myList->next[ indexToPush ];
    myList->next[ indexToPush ] = (int)freeIndexNow;

    // ARRAY PREV
    myList->prev[ freeIndexNow ] = (int)indexToPush;
    myList->prev[ myList->next[ freeIndexNow ] ] = (int)freeIndexNow;

    ++(myList->countOfElement);

    return (int)freeIndexNow;
}

listErrors listDelete( List* myList, size_t indexToDelete ){
    CHECK_LIST( myList, DELETE_ERROR );

    if( myList->countOfElement == 1){
        colorPrintf( NOMODE, RED, "\nNot enough element in list\n");
        return NOT_ENOUGH_ELEMENT;
    }

    myList->next[ myList->prev[ indexToDelete + 1 ] ] = myList->next[ indexToDelete + 1  ];
    myList->prev[ myList->next[ indexToDelete + 1  ] ] = myList->prev[ indexToDelete + 1  ];
    myList->next[ indexToDelete + 1] = (int)myList->freeIndex;
    myList->prev[ indexToDelete + 1 ] = -1;
    myList->freeIndex = indexToDelete + 1;

    return CORRECT_LIST;
}
void printList( List* myList ){
    CHECK_LIST( myList, (void)0 );

    printf("\nheadIndex = %d\n", myList->next[ headPosition ] );
    printf("tailIndex = %d\n", myList->prev[ tailPosition ] );
    printf( "countOfElement =  %lu\n", myList->countOfElement );
    for( size_t indexList = (size_t)myList->next[ headPosition ]; indexList != 0; indexList = (size_t)myList->next[ indexList ] ){
        printf( "[%lu] = " listFormat " ", indexList, myList->data[ indexList ] );
    }
    printf("\n");
    for( size_t indexNext = 0; indexNext < myList->sizeOfList; indexNext++ ){
        printf( "[%lu] = %d ", indexNext, myList->next[ indexNext ] );
    }
    printf("\n");
    for( size_t indexPrev = 0; indexPrev < myList->sizeOfList; indexPrev++ ){
        printf( "[%lu] = %d ", indexPrev, myList->prev[ indexPrev ] );
    }
    printf("\n");
    printf("\nfreeIndex = %lu\n", myList->freeIndex );
}

listErrors reallocateList( List* myList, size_t indexToPush ){
    CHECK_LIST( myList, REALLOCATE_ERROR );

    if( indexToPush < myList->sizeOfList ){
        return CORRECT_LIST;
    }

    myList->sizeOfList *= 2;

    myList->data = (listValue*)realloc( myList->data, sizeof( listValue ) * myList->sizeOfList );
    if( myList->data == NULL ){
        return DATA_NULL_PTR;
    }
    myList->data[ myList->sizeOfList - 1 ] = canary;

    myList->next = (int*)realloc( myList->next, sizeof( int ) * myList->sizeOfList );
    if( myList->next == NULL ){
        return NEXT_NULL_PTR;
    }

    myList->prev = (int*)realloc( myList->prev, sizeof( int ) * myList->sizeOfList );
    if( myList->data == NULL ){
        return PREV_NULL_PTR;
    }

    return CORRECT_LIST;
}

int getHeadIndex( List* myList ){
    return myList->next[ headPosition ];
}

int getTailIndex( List* myList ){
    return myList->prev[ tailPosition ];
}

int findElement( List* myList, listValue numberThatNeedFind ){

    for( int dataIndex = 0; dataIndex < (int)myList->countOfElement; dataIndex++ ){
        if( abs(myList->data[ dataIndex ] - numberThatNeedFind) < epsilon ){
            return dataIndex;
        }
    }

    return -1;
}
