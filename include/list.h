#ifndef H_LIST
#define H_LIST

static const size_t startSizeForArray = 20;
static const double canary = 179171341; // 0xAADF00D

enum listErrors {
    CORRECT                 = 0,
    LIST_NULL_PTR           = 1,
    DATA_INITIALIZATION_ERR = 2,
    NEXT_INITIALIZATION_ERR = 3,
    PREV_INITIALIZATION_ERR = 4,
    DATA_NULL_PTR           = 5,
    NEXT_NULL_PTR           = 6,
    PREV_NULL_PTR           = 7,
    ERROR_OPEN_FILE         = 8,
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

listErrors initList( List* myList );

#define CHECK_PTR( ptr, err )       \
    if( ptr == NULL ){              \
        return err;                 \
    }

void destroyList( List* myList );

int listInsert( List* myList, double number , size_t indexToPush );

void listDelete( List* myList, size_t indexToDelete );

listErrors isEnoughSpaceInList( List* myList, size_t indexToPush );

void printList( List* myList );

listErrors dumpList( List* myList, const char* nameOfGraphFile );

#endif
