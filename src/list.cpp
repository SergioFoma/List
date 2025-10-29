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
    myList->headIndex = 0;
    myList->tailIndex = 0;
    myList->sizeOfList = startSizeForArray;
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
    myList->sizeOfList = 0;

    myList->data = NULL;
    myList->next = NULL;
    myList->prev = NULL;
}

int listInsert( List* myList, double number , size_t indexToPush ){
    CHECK_PTR( myList, 0 );

    listErrors statusOfMemoryRealloc = isEnoughSpaceInList( myList, indexToPush );
    if( statusOfMemoryRealloc != CORRECT ){
        return -1;
    }

    size_t freeIndexNow = myList->freeIndex ;
    myList->freeIndex = myList->next[ freeIndexNow ];

    myList->data[ freeIndexNow ] = number;

    // ARRAY NEXT
    myList->next[ freeIndexNow ] = myList->next[ indexToPush ];
    if( indexToPush != 0 ){
        myList->next[ indexToPush ] = freeIndexNow;
    }
    else if( freeIndexNow != 1 ){
        myList->next[ freeIndexNow ] = myList->headIndex;
    }

    // ARRAY PREV
    myList->prev[ freeIndexNow ] = indexToPush;
    if( indexToPush != 0 && myList->next[ freeIndexNow ] != 0 ){
        myList->prev[ myList->next[ freeIndexNow ] ] = freeIndexNow;
    }
    else if( freeIndexNow != 1 && indexToPush == 0 ){
        myList->prev[ freeIndexNow ] = myList->prev[ myList->headIndex ];
        myList->prev[ myList->headIndex ] = freeIndexNow;
    }

    // headIndex && tailIndex

    if( indexToPush == 0){
        myList->headIndex = freeIndexNow;
    }
    if( myList->next[ freeIndexNow ] == 0 ){
        myList->tailIndex = freeIndexNow;
    }

    printf("\nindexTOPush = %lu\n", indexToPush );
    printList( myList );
    return (int)freeIndexNow;
}

void listDelete( List* myList, size_t indexToDelete ){
    if( indexToDelete == 0 ){
        myList->headIndex = myList->next[ indexToDelete ] ;
    }
    else{
        myList->next[ myList->prev[ indexToDelete ] ] = myList->next[ indexToDelete ];
        myList->prev[ myList->next[ indexToDelete ] ] = myList->prev[ indexToDelete ];
    }

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
}

listErrors isEnoughSpaceInList( List* myList, size_t indexToPush ){
    if( indexToPush < myList->sizeOfList ){
        return CORRECT;
    }

    myList->sizeOfList *= 2;

    myList->data = (double*)realloc( myList->data, sizeof( double ) * myList->sizeOfList );
    if( myList->data == NULL ){
        return DATA_NULL_PTR;
    }

    myList->next = (int*)realloc( myList->next, sizeof( int ) * myList->sizeOfList );
    if( myList->next == NULL ){
        return NEXT_NULL_PTR;
    }

    myList->prev = (int*)realloc( myList->prev, sizeof( int ) * myList->sizeOfList );
    if( myList->data == NULL ){
        return PREV_NULL_PTR;
    }

    return CORRECT;
}

listErrors dumpList( List* myList, const char* nameOfGraphFile ){
    FILE* graphFile = fopen( nameOfGraphFile, "w" );
    if( graphFile == NULL ){
        return ERROR_OPEN_FILE;
    }

    fprintf( graphFile, "digraph G{\n"
                        "\trankdir = TB;\n"
                        "\tnode[color = \"red\", fontsize = 14];\n"
                        "\tedge[color = \"darkgreen\", fontsize = 12];\n"
            );

    for( size_t physIndex = 1; physIndex < myList->freeIndex; physIndex++ ){
        fprintf( graphFile, "\tnode%lu [shape=\"Mrecord\"; style =\"filled\"; fillcolor =\"azure3\"; label = "
                            "\"{ physIndex = %lu | elem = %lg | prev = %d | next = %d }\"];\n",
                            physIndex, physIndex, myList->data[ physIndex ], myList->prev[ physIndex ], myList->next[ physIndex ] );
    }

    for( size_t dataIndex = 1; dataIndex < myList->freeIndex - 1; dataIndex++ ){
        fprintf( graphFile, "\tnode%lu -> node%lu[color = \"grey100\"];\n"
                            /*"\tnode%lu -> node%lu[ color = \"grey100\"];\n"*/,
                            dataIndex, dataIndex + 1/*, dataIndex + 1, dataIndex*/ );
    }

    for( size_t nextIndex = myList->headIndex; myList->next[ nextIndex ]!= 0; nextIndex = myList->next[ nextIndex ] ){
        fprintf( graphFile, "\tnode%lu -> node%d[color = \"green\"];\n", nextIndex, myList->next[ nextIndex ] );
    }

    /*for( size_t prevIndex = myList->tailIndex; myList->prev[ prevIndex ] != 0; prevIndex = myList->prev[ prevIndex ] ){
        fprintf( graphFile, "\tnode%lu -> node%d[color = \"darkorchid1\"];\n", prevIndex, myList->prev[ prevIndex ] );
    }*/

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
    for( size_t nextIndex = 1; nextIndex < myList->freeIndex; nextIndex++ ){
        fprintf( htmlDump, "%d ", myList->next[ nextIndex ] );
    }
    fprintf( htmlDump, "\nPrev: \n" );

    for( size_t prevIndex = 1; prevIndex < myList->freeIndex; prevIndex++ ){
        fprintf( htmlDump, "%d ", myList->prev[ prevIndex ] );
    }

    fprintf( htmlDump, "\n\nImage:\n"
                       "<img src=GRAPHFILE.png width = 400px>\n\n");

    fclose( htmlDump );
    return CORRECT;
}
