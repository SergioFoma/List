#ifndef H_LIST
#define H_LIST

static const size_t startSizeForArray = 20;
static const double canary = 179171341; // 0xAADF00D

enum listErrors {
    CORRECT                 = 0,
    LIST_NULL_PTR           = 1,
    DATA_INITIALIZATION_ERR = 2,
    NEXT_INITIALIZATION_ERR = 3,
    PREV_INITIALIZATION_ERR = 4
};

struct List{
    double* data;
    int* next;
    int* prev;
    size_t freeIndex;
    size_t headIndex;
    size_t tailIndex;
};

listErrors initList( List* myList );

#define CHECK_PTR( ptr, err )       \
    if( ptr == NULL ){              \
        return err;                 \
    }

void destroyList( List* myList );

size_t listInsert( List* myList, double number , size_t indexToPush );

void printList( List* myList );

#endif
