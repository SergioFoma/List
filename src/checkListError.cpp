#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "checkListError.h"

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

    for( size_t physIndex = 1; physIndex < myList->countOfElement; physIndex++ ){
        if( myList->prev[ myList->next[ physIndex ] ] != (int)physIndex ){
            return ONE_WAY_COMMUNICATION;
        }
    }

    if( myList->countOfElement == 1 ){
        return CORRECT_LIST;
    }

    int oneStepPtr = myList->next[ headPosition ];
    int secondStepPtr = myList->next[ headPosition ];

    while( oneStepPtr < ( myList->countOfElement - 1 ) ){
        if( oneStepPtr == secondStepPtr && oneStepPtr != myList->next[ headPosition ] ){
            return LIST_HAVE_CYCLE;
        }
        
        oneStepPtr = myList->next[ oneStepPtr ];

        if( myList->next[ secondStepPtr] != myList->sizeOfList - 1 ){
            secondStepPtr = myList->next[ myList->next[ secondStepPtr ] ];
        }
    }

    return CORRECT_LIST;
}

listErrors dumpList( List* myList ){
    listErrors listErr = listVerify( myList );

    if( listErr != CORRECT_LIST ){
        return listErr;
    }

    FILE* graphFile = fopen( "GRAPHFILE.txt", "w" );
    if( graphFile == NULL ){
        return ERROR_OPEN_FILE;
        fclose( graphFile );
    }

    fprintf( graphFile, "digraph G{\n"
                        "\trankdir = TB;\n"
                        "\tnode[shape = \"hexagon\", color = \"black\", fontsize = 14, shape = record ];\n"
                        "\tedge[color = \"red\", fontsize = 12];\n"
                        "\tnode0 [shape=\"plain\"; style =\"filled\"; fillcolor =\"hotpink2\"; color = \"red\"; label = "
                        "<<table><tr><td> physIndex = %lu </td></tr> <tr><td> elem = canary </td></tr> <tr><td> prev = %d </td></tr> <tr><td> next = %d </td></tr></table>>];\n"
                        "\tnode%lu [shape=\"plain\"; style =\"filled\"; fillcolor =\"hotpink2\"; color = \"red\"; label = "
                        "<<table><tr><td> physIndex = %lu </td></tr> <tr><td> elem = canary </td></tr> <tr><td> prev = %d </td></tr> <tr><td> next = %d </td></tr></table>>];\n",
                        headPosition, myList->prev[ tailPosition ], myList->next[ headPosition ],
                        myList->sizeOfList - 1, myList->sizeOfList - 1, myList->prev[ myList->sizeOfList - 1 ], myList->next[ myList->sizeOfList - 1 ]
            );

    for( size_t physIndex = 1; physIndex < myList->countOfElement; physIndex++ ){

        fprintf( graphFile, "\tnode%lu -> node%d[style = \"invis\", dir = \"both\"];\n",
                            physIndex, myList->next[ physIndex ] );

    }
    for( size_t physIndex = 1; physIndex < myList->countOfElement; physIndex++ ){
        if( myList->next[ headPosition ] == (int)physIndex ){
            fprintf( graphFile, "\tHEAD [shape=\"box\"; style =\"filled\"; fillcolor =\"hotpink2\"; color = \"red\"];\n"
                                "\t{\n"
                                "\t\trank = same;\n"
                                "\t\tHEAD;\n"
                                "\t\tnode%lu;\n"
                                "\t}\n"
                                "\tHEAD -> node%lu[color = \"blue\"];\n"
                                , physIndex, physIndex );
        }
        else if( myList->prev[ tailPosition ] == (int)physIndex ){
            fprintf( graphFile, "\tTAIL [shape=\"box\"; style =\"filled\"; fillcolor =\"hotpink2\"; color = \"red\"];\n"
                                "\t{\n"
                                "\t\trank = same;\n"
                                "\t\tTAIL;\n"
                                "\t\tnode%lu;\n"
                                "\t}\n"
                                "\tTAIL -> node%lu[color = \"blue\"];\n"
                                , physIndex, physIndex );
        }

        if( myList->prev[ myList->next[ physIndex ] ] == (int)physIndex ){
            fprintf( graphFile, "\tnode%lu [shape=\"plain\"; style =\"filled\"; fillcolor =\"azure3\"; label = "
                            "<<table><tr><td> physIndex = %lu</td></tr> <tr><td> elem = " listFormat " </td></tr> <tr><td> prev = %d </td></tr> <tr><td> next = %d </td></tr></table>>; color = \"red\"];\n",
                             physIndex, physIndex, myList->data[ physIndex ], myList->prev[ physIndex ], myList->next[ physIndex ] );
            fprintf( graphFile, "\tnode%lu -> node%d[color = \"darkorchid1\", dir = \"both\" ];\n", physIndex, myList->next[ physIndex ] );
        }
        else if( myList->prev[ physIndex ] != -1 && myList->next[ physIndex ] != 0 ){
            fprintf( graphFile, "\tnode%lu -> node%d[color = \"red\"];\n"
                                "\tnode%d -> node%d[color = \"orange\"];\n",
                                physIndex, myList->next[ physIndex ], myList->next[ physIndex ], myList->prev[ myList->next[ physIndex ] ] );
        }
    }

    fprintf( graphFile, "\tFREE_ELEMENTS [shape=\"box\"; style =\"filled\"; fillcolor =\"hotpink2\"; color = \"red\"];\n"
                        "\t{\n"
                        "\t\trank = same;\n"
                        "\t\tFREE_ELEMENTS;\n"
                        "\t\tnode%lu;\n"
                        "\t}\n"
                        "\tFREE_ELEMENTS -> node%lu[color = \"blue\"];\n", myList->freeIndex, myList->freeIndex );
    for( size_t freeDataIndex = myList->freeIndex; myList->next[ freeDataIndex ] != 0 ; freeDataIndex = (size_t)myList->next[ freeDataIndex ] ){
        fprintf( graphFile, "\tnode%lu [shape=\"plain\"; style =\"filled\"; fillcolor =\"orange\"; label = "
                            "<<table><tr><td> physIndex = %lu </td></tr> <tr><td> freeElement </td></tr> <tr><td> prev = %d </td></tr> <tr><td> next = %d </td></tr></table>>; color = \"red\"];\n",
                            freeDataIndex, freeDataIndex, myList->prev[ freeDataIndex ], myList->next[ freeDataIndex ] );
    }
    for( size_t freeDataIndex = myList->freeIndex; myList->next[ freeDataIndex ] != 0; freeDataIndex = (size_t)myList->next[ freeDataIndex ] ){
        fprintf( graphFile, "\tnode%lu -> node%d[color = \"pink\"];\n", freeDataIndex, myList->next[ freeDataIndex ] );
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
                       "<h1> <font color=red> LIST DUMP </font>  </h1>"
                       "\n\n"
                       "<h1> List { %s:%s:%d } </h1>\n\n", __FILE__, __func__, __LINE__ );

    fprintf( htmlDump, "<h2 style = \"color: green;\">next: \n");
    for( size_t nextIndex = 0; nextIndex < myList->countOfElement; nextIndex++ ){
        fprintf( htmlDump, "%d ", myList->next[ nextIndex ] );
    }
    fprintf( htmlDump, "</h2>\n\n<h2 style = \"color: orange;\">Prev: \n" );

    for( size_t prevIndex = 0; prevIndex < myList->countOfElement; prevIndex++ ){
        fprintf( htmlDump, "%d ", myList->prev[ prevIndex ] );
    }

    fprintf( htmlDump, "</h2>\n\n<h1>Image:</h1>\n"
                       "<img src=GRAPHFILE.png width = 700px>\n\n");

    fclose( htmlDump );
    return CORRECT_LIST;
}
