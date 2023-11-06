#ifndef LIST_H
#define LIST_

#define CHECK_MALLOC(ptr)                               \
    if (ptr == NULL) {                                  \
        printf("Ошибка выделения памяти.\n");           \
    }

#define VERIFY_VALUE(scr, operation, SCR)               \
    if (list->scr operation) {                          \
        sum_errors = sum_errors | ERROR_##SCR##_BIT;    \
    }

#define PRINT_ERRORS(scr, SCR)                          \
    if (sum_errors & ERROR_##SCR##_BIT)                 \
        fprintf(LOG_FILE, "error %s\n", #scr);

const int SIZE_DATA      = 20;
const int NOW_DATA       = 10;
const int WANT_COUNT_ADD = 10;
const int FREE_ELEMENT   = -1;
const int ZERO_ELEMENT   = 0;

void CtorList (struct List* list);
void PushElement (struct List* list, int index, int value);
void PopElement (struct List* list, int index);
void DtorList (struct List* list);

void Verify (struct List* list);
void DumpList (struct List* list);

void GenerateImage (struct List* list);

struct List {
    int *data;
    int *next;
    int *prev;
    int free;
    int size;
};

enum ListErrors {
    ERROR_DATA_BIT    = 1,
    ERROR_NEXT_BIT    = 1 << 2,
    ERROR_PREV_BIT    = 1 << 3,
    ERROR_FREE_BIT    = 1 << 4,
    ERROR_HEAD_BIT    = 1 << 5,
    ERROR_TAIL_BIT    = 1 << 6,
    ERROR_SIZE_BIT    = 1 << 7,
    ERROR_MEANING_BIT = 1 << 8
};
#endif
