#define CHECK_MALLOC(ptr)                       \
    if (ptr == NULL) {                          \
        printf("Ошибка выделения памяти.\n");   \
        return; \
    }

#define FILL_ARRAY_FROM(arr, scr)               \
    while (i >= 0 && count < list->size) {      \
        arr[count] = list->data[i];             \
        count++;                                \
        i = list->scr[i];                       \
    }                                           \

const int SIZE_DATA = 20;
const int NOW_DATA = 10;
const int WANT_COUNT_ADD = 10;

void CtorList (struct List* list);
void PushElement (struct List* list, int index, int value);
void PopElement (struct List* list, int index);
void DumpList(struct List* list);
void Partion(FILE* file, struct List* list);
void DtorList (struct List* list);
void VerifyList (struct List* list);

void InitializationNext (struct List* list);
void InitializationPrev (struct List* list);
void InitializationArr (int* arr, int size);

void MyIniz (struct List* list);
void AddingElementAfter0 (struct List* list, int value);

void VerifyList (struct List* list);

struct List {
    int *data;
    int *next;
    int *prev;
    int free;
    int head;
    int tail;
    int size;
};

