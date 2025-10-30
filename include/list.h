#ifndef H_LIST
#define H_LIST

static const size_t startSizeForArray = 20;
static const double canary = 179171341; // 0xAADF00D

enum listErrors {
    CORRECT_LIST            = 0,
    LIST_NULL_PTR           = 1,
    LIST_INITIALIZATION_ERR = 2,
    DATA_INITIALIZATION_ERR = 3,
    NEXT_INITIALIZATION_ERR = 4,
    PREV_INITIALIZATION_ERR = 5,
    DATA_NULL_PTR           = 6,
    NEXT_NULL_PTR           = 7,
    PREV_NULL_PTR           = 8,
    ERROR_OPEN_FILE         = 9,
    SIZE_LIST_ERROR         = 10,
    REALLOCATE_ERROR        = 11,
    DUMP_ERROR              = 12
};

struct List{
    double* data;
    int* next;
    int* prev;
    size_t freeIndex;
    size_t headIndex;
    size_t tailIndex;
    size_t sizeOfList;
};

#define CHECK_PTR( ptr, err )       \
    if( ptr == NULL ){              \
        return err;                 \
    }

listErrors initList( List* myList );

void destroyList( List* myList );

int listInsert( List* myList, double number , size_t indexToPush );

void listDelete( List* myList, size_t indexToDelete );

listErrors reallocateList( List* myList, size_t indexToPush );

void printList( List* myList );

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
