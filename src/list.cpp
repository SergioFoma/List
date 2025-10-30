#include <stdio.h>
#include <stdlib.h>

#include "list.h"
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
        myList->next[ nextIndex ] = nextIndex + 1;
    }
    myList->next[ startSizeForArray - 1 ] = 0;

    myList->prev[ tailPosition ] = 0;
    for( size_t prevIndex = 1; prevIndex < startSizeForArray; prevIndex++ ){
        myList->prev[ prevIndex ] = -1;
    }
    return CORRECT_LIST;
}

void destroyList( List* myList ){
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

    size_t freeIndexNow = myList->freeIndex ;
    myList->freeIndex = myList->next[ freeIndexNow ];

    myList->data[ freeIndexNow ] = number;

    // ARRAY NEXT
    myList->next[ freeIndexNow ] = myList->next[ indexToPush ];
    myList->next[ indexToPush ] = freeIndexNow;

    // ARRAY PREV
    myList->prev[ freeIndexNow ] = indexToPush;
    myList->prev[ myList->next[ freeIndexNow ] ] = freeIndexNow;

    ++(myList->countOfElement);

    printf("\nindexToPush = %lu\n", indexToPush );
    printList( myList );
    return (int)freeIndexNow;
}

listErrors listDelete( List* myList, size_t indexToDelete ){
    CHECK_LIST( myList, DELETE_ERROR );

    if( myList->countOfElement == 1){
        colorPrintf( NOMODE, RED, "\nNot enough element in list\n");
        return NOT_ENOUGH_ELEMENT;
    }
    myList->next[ myList->prev[ indexToDelete + 1 ] ] = myList->next[ indexToDelete + 1 ];
    myList->prev[ myList->next[ indexToDelete + 1 ] ] = myList->prev[ indexToDelete + 1 ];

}
void printList( List* myList ){
    CHECK_LIST( myList, (void)0 );

    printf("\nheadIndex = %d\n", myList->next[ headPosition ] );
    printf("tailIndex = %d\n", myList->prev[ tailPosition ] );
    printf( "countOfElement =  %lu\n", myList->countOfElement );
    for( size_t indexList = myList->next[ headPosition ]; indexList != 0; indexList = myList->next[ indexList ] ){
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

listErrors dumpList( List* myList ){
    CHECK_LIST( myList, DUMP_ERROR );

    FILE* graphFile = fopen( "GRAPHFILE.txt", "w" );
    if( graphFile == NULL ){
        return ERROR_OPEN_FILE;
        fclose( graphFile );
    }

    fprintf( graphFile, "digraph G{\n"
                        "\trankdir = TB;\n"
                        "\tnode[color = \"red\", fontsize = 14];\n"
                        "\tedge[color = \"darkgreen\", fontsize = 12];\n"
            );

    for( size_t physIndex = 1; physIndex < myList->countOfElement; physIndex++ ){
        fprintf( graphFile, "\tnode%lu [shape=\"Mrecord\"; style =\"filled\"; fillcolor =\"azure3\"; label = "
                            "\"{ physIndex = %lu | elem = " listFormat " | prev = %d | next = %d }\"];\n",
                            physIndex, physIndex, myList->data[ physIndex ], myList->prev[ physIndex ], myList->next[ physIndex ] );
    }

    for( size_t nextIndex = myList->next[ headPosition ]; myList->next[ nextIndex ]!= 0; nextIndex = myList->next[ nextIndex ] ){
        fprintf( graphFile, "\tnode%lu -> node%d[color = \"green\"];\n", nextIndex, myList->next[ nextIndex ] );
    }

    for( size_t prevIndex = myList->prev[ tailPosition ]; myList->prev[ prevIndex ] != 0; prevIndex = myList->prev[ prevIndex ] ){
        fprintf( graphFile, "\tnode%lu -> node%d[color = \"darkorchid1\"];\n", prevIndex, myList->prev[ prevIndex ] );
    }

    fprintf( graphFile, "}" );
    fclose( graphFile );

    system("dot GRAPHFILE.txt -Tpng -o GRAPHFILE.png");

    // HTML

    FILE* htmlDump = fopen( "GRAPHFILE.html", "w" );
    if( htmlDump == NULL ){
        return ERROR_OPEN_FILE;
    }

    fprintf( htmlDump, "<pre>\n"
                       "<h3> <font color=red> LIST DUMP </font>  <h3>"
                       "\n\n"
                       "List { %s:%s:%d }\n\n", __FILE__, __func__, __LINE__ );

    fprintf( htmlDump, "next: \n");
    for( size_t nextIndex = 1; nextIndex < myList->countOfElement; nextIndex++ ){
        fprintf( htmlDump, "%d ", myList->next[ nextIndex ] );
    }
    fprintf( htmlDump, "\nPrev: \n" );

    for( size_t prevIndex = 1; prevIndex < myList->countOfElement; prevIndex++ ){
        fprintf( htmlDump, "%d ", myList->prev[ prevIndex ] );
    }

    fprintf( htmlDump, "\n\nImage:\n"
                       "<img src=GRAPHFILE.png width = 400px>\n\n");

    fclose( htmlDump );
    return CORRECT_LIST;
}

listErrors listVerify( List* myList ){
    if( myList == NULL ){
        return LIST_NULL_PTR;
    }
    else if( myList->data == NULL ){
        return DATA_NULL_PTR;
    }
    else if( myList->next == NULL ){
        return NEXT_NULL_PTR;
    }
    else if( myList->prev == NULL ){
        return PREV_NULL_PTR;
    }

    if( myList->sizeOfList == 0 ){
        return SIZE_LIST_ERROR;
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

    for( size_t index = 0; index < myList->countOfElement; index++ ){
        if( myList->data[ index ] == numberThatNeedFind ){
            return index;
        }
    }

    return -1;
}
