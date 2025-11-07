#ifndef H_DUMPLIST
#define H_DUMPLIST

#include "list.h"

listErrors dumpList( List* myList );

#ifdef NDEBUG
#define CHECK_LIST( List, typeOfError )do{}while(false)
#else

listErrors listVerify( List* myList );

#define CHECK_LIST( myList, typeOfError )                                \
    do{                                                                  \
        if( listVerify( myList ) != CORRECT_LIST ){                      \
            ( dumpList( myList ) );                                      \
            return typeOfError;                                          \
        }                                                                \
    }while(false)

#endif

#endif
