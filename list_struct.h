#ifndef LIST_STRUCT_H
#define LIST_STRUCT_H

#define VERIFY_VALUE(scr, operation, SCR)               \
    if (list->scr operation) {                          \
        sum_errors = sum_errors | ERROR_##SCR##_BIT;    \
    }

#define CHECK_MALLOC(ptr)                               \
    if (ptr == NULL) {                                  \
        printf("Ошибка выделения памяти.\n");           \
    }

#define PRINT_ERRORS(scr, SCR)                          \
    if (sum_errors & ERROR_##SCR##_BIT)                 \
        fprintf(LOG_FILE, "error %s\n", #scr);

enum ListErrors {
    ERROR_SIZE_BIT    = 1,
    ERROR_MEANING_BIT = 1 << 2
};

const int SIZE_DATA      = 20;
const int NOW_DATA       = 10;
const int WANT_COUNT_ADD = 10;
const int FREE_ELEMENT   = -1;
const int ZERO_ELEMENT   = 0;

struct Node {
    int value;
    Node* next;
    Node* prev;
};

struct List {
    Node* fixedElement;
    int size;
};

void CtorList(struct List* list);
void DtorList(List* list);
void PushElement(List* list, Node* position, int value, Node** insertedValPtr);
void PopElement(List* list, Node* position);

void DumpList (struct List* list);

void CreateNode(FILE* dotFile, Node* id, const int value,
                               Node* next, Node* prev);
void GenerateImage (struct List* list);
void CreateNewGraph();

Node* GetHead(List* list);
Node* GetTail(List* list);
#endif
