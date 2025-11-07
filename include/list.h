#ifndef H_LIST
#define H_LIST

typedef double listValue;
#define listFormat "%lg"

static const size_t startSizeForArray = 20;
static const listValue canary = 179171341;  // 0xAADF00D
const size_t headPosition = 0;              // next[0]
const size_t tailPosition = 0;              // prev[0]
const double epsilon = 1e-6;

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
    DUMP_ERROR              = 12,
    NOT_ENOUGH_ELEMENT      = 13,
    DELETE_ERROR            = 14,
    ONE_WAY_COMMUNICATION   = 15,
    LIST_HAVE_CYCLE         = 16,
};

struct List{
    listValue* data;
    int* next;
    int* prev;
    size_t freeIndex;
    size_t sizeOfList;
    size_t countOfElement;
};

#define CHECK_PTR( ptr, err )       \
    if( ptr == NULL ){              \
        return err;                 \
    }

listErrors initList( List* myList );

void destroyList( List* myList );

int listInsert( List* myList, listValue number , size_t indexToPush );

listErrors listDelete( List* myList, size_t indexToDelete );

int findElement( List* myList, listValue numberThatNeedFind );

int getHeadIndex( List* myList );

int getTailIndex( List* myList );

listErrors reallocateList( List* myList, size_t indexToPush );

void printList( List* myList );

#endif
