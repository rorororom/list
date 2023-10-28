const int SIZE_DATA = 20;
const int NOW_DATA = 10;

void CtorList (struct List* list);
void PushElement (struct List* list, int index, int value);
void PopElement (struct List* list, int index);
void DumpList(struct List* list);
void Partion(FILE* file, struct List* list);
void DtorList (struct List* list);

void InitializationNext (struct List* list);
void InitializationPrev (struct List* list);

void MyIniz (struct List* list);
void AddingElementAfter0 (struct List* list, int value);

struct List {
    int *data;
    int *next;
    int *prev;
    int free;
    int head;
    int tail;
};
