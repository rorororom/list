#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "log_funcs.h"
#include "list.h"

int main()
{
    OpenLogFile("LOGE.log", "w");

    struct List list = {};

    CtorList(&list);

    InitializationNext (&list);
    InitializationPrev (&list);

    DumpList(&list);

    for (int i = 0; i < WANT_COUNT_ADD; i++)
    {
        PushElement(&list, i, 10 * (i+1));
    }

    PopElement(&list, 3);
    PopElement(&list, 5);
    PopElement(&list, 1);

    VerifyList(&list);
    DtorList(&list);
}

void CtorList (struct List* list)
{
    list->free = 1;
    list->head = 0;
    list->tail = 0;
    list->size = 0;

    list->data = (int*)calloc(SIZE_DATA, sizeof(int));
    CHECK_MALLOC(list->data);
    list->next = (int*)calloc(SIZE_DATA, sizeof(int));
    CHECK_MALLOC(list->next);
    list->prev = (int*)calloc(SIZE_DATA, sizeof(int));
    CHECK_MALLOC(list->prev);

    InitializationNext (list);
    InitializationPrev (list);
    InitializationArr (list->data, SIZE_DATA);

    list->data[0] = 0;
    list->next[0] = 0;
    list->prev[0] = 0;
}

void DtorList (struct List* list)
{
    assert(list != nullptr);
    for (int i = 0; i < SIZE_DATA; i++)
    {
        list->data[i] = 0;
        list->next[i] = 0;
        list->prev[i] = 0;
    }

    free(list->data);
    free(list->next);
    free(list->prev);

    list->free = -1;
    list->head = -1;
    list->tail = -1;
    list->size = -1;
}

void PushElement (struct List* list, int index, int value)
{
    assert(list != nullptr);
    assert(index >= 0 && index < SIZE_DATA);

    if (index == 0)
    {
        AddingElementAfter0 (list, value);
        list->size++;
        return;
    }

    int nowIndex = abs(list->free);
    list->free = abs(list->next[nowIndex]);
    list->data[nowIndex] = value;

    if (index == list->tail)
    {
        list->tail = abs(nowIndex);

        list->next[nowIndex] = 0;
    }
    else
    {
        list->next[nowIndex] = list->next[index]; //записываем следующий элемент для нового элемента
                                                  //(это следующий элемент старого элемента)
        list->prev[list->next[nowIndex]] = nowIndex; //предыдущий следующего нового - это индекс нового
    }

    list->next[index] = nowIndex; //следующий элемент индекса, это индекс нового элемента
    list->prev[nowIndex] = index; //предыдущий элемент для нового это индекс

    list->size++;
    DumpList(list);
    return;
}

void PopElement (struct List* list, int index)
{
    assert(index >= 0 && index < SIZE_DATA);
    assert(list != nullptr);

    list->prev[list->next[index]] = list->prev[index];
    list->next[list->prev[index]] = list->next[index];

    if (list->head == index)
    {
        list->head = list->next[index];
    }
    if (list->tail == index)
    {
        list->tail = list->prev[index];
    }

    list->next[index] = -list->free;
    list->prev[index] = list->prev[abs(list->free)];

    list->data[index] = 0;
    list->free = index;

    list->size--;
    DumpList(list);
}

void InitializationNext (struct List* list)
{
    assert(list != nullptr);

    for (int i = 1; i < SIZE_DATA; i++)
    {
        if (i == SIZE_DATA - 1) list->next[i] = 0;
        else list->next[i] =  -(i+1);
    }
}

void InitializationPrev (struct List* list)
{
    assert(list != nullptr);

    for (int i = 1; i < SIZE_DATA; i++)
    {
        if (i == 1) list->prev[i] = 0;
        else list->prev[i] = -1;
    }
}

void InitializationArr (int* arr, int size)
{
    assert(arr != nullptr);

    for (int i = 0; i < size; i++)
    {
        arr[i] = 0;
    }
}

void MyIniz (struct List* list)
{
    assert(list != nullptr);

    for (int i = 1; i < NOW_DATA; i++)
    {
        list->data[i] = i * 10;

        if (i == NOW_DATA - 1)
        {
            list->next[i] = 0;
            list->free = -(i + 1);
            list->prev[i] = i - 1;
        }
        else if (i == 1)
        {
            list->prev[i] = 0;
            list->next[i] = list->free + 1;
            list->free++;
        }
        else
        {
            list->next[i] = list->free + 1;
            list->prev[i] = i - 1;
            list->free++;
        }

        printf("%d\n", list->data[i]);
        printf("%d\n\n", list->next[i]);
    }

    list->tail = NOW_DATA - 1;
    list->head = 1;
    DumpList(list);
}

void DumpList (struct List* list)
{
    assert(list != nullptr);

    fprintf(LOG_FILE, "index: ");
    for (int i = 0; i < SIZE_DATA; i++)
    {
        fprintf(LOG_FILE, "%.3d_|_", i);
    }

    Partion(LOG_FILE, list);

    fprintf(LOG_FILE, "\ndata: ");
    for (int i = 0; i < SIZE_DATA; i++)
    {
        fprintf(LOG_FILE, " %.3d |", list->data[i]);
    }

    Partion(LOG_FILE, list);

    fprintf(LOG_FILE, "\nnext: ");
    for (int i = 0; i < SIZE_DATA; i++)
    {
        if (list->next[i] < 0) fprintf(LOG_FILE, "%.3d |", list->next[i]);
        else fprintf(LOG_FILE, " %.3d |", list->next[i]);
    }

    Partion(LOG_FILE, list);

    fprintf(LOG_FILE, "\nprev: ");
    for (int i = 0; i < SIZE_DATA; i++)
    {
        if (list->prev[i] < 0) fprintf(LOG_FILE, "%.3d |", list->prev[i]);
        else fprintf(LOG_FILE, " %.3d |", list->prev[i]);
    }
    Partion(LOG_FILE, list);


    fprintf(LOG_FILE, "\n\nhead: [%.3d]\n", list->head);
    fprintf(LOG_FILE, "tale: [%.3d]\n", list->tail);
    fprintf(LOG_FILE, "free: [%.3d]\n", list->free);

}

void Partion(FILE* file, struct List* list)
{
    assert(list != nullptr);

    fprintf(file, "\n           ");
    for (int i = 0; i < SIZE_DATA - 1; i++)
    {
        fprintf(file, "|_____");
    }
}

void AddingElementAfter0 (struct List* list, int value)
{
    assert(list != nullptr);

    list->data[1] = value;
    list->next[1] = 0;
    list->prev[1] = 0;

    list->head = 1;
    list->tail = 1;
    list->free = 2;

    DumpList(list);
}

void VerifyList(struct List* list)
{
    assert(list != nullptr);

    int* arrNext = (int*)malloc(list->size * sizeof(int));
    CHECK_MALLOC(arrNext);
    int* arrPrev = (int*)malloc(list->size * sizeof(int));
    CHECK_MALLOC(arrPrev);

    InitializationArr (arrNext, list->size);
    InitializationArr (arrPrev, list->size);

    int i = list->head;
    int count = 0;
    FILL_ARRAY_FROM(arrNext, next);

    i = list->tail;
    count = 0;
    FILL_ARRAY_FROM(arrPrev, prev);

    count = 0;
    while (count < list->size / 2)
    {
        int temp = arrPrev[count];
        arrPrev[count] = arrPrev[list->size - 1 - count];
        arrPrev[list->size - 1 - count] = temp;
        count++;
    }

    int arraysMatch = 1;
    for (int j = 0; j < list->size; j++)
    {
        if (arrNext[j] != arrPrev[j])
        {
            arraysMatch = 0;
            break;
        }
    }

    if (arraysMatch)
    {
        fprintf(LOG_FILE, "Массивы arrNext и arrPrev совпадают.\n");
    }
    else
    {
        printf("Массивы arrNext и arrPrev не совпадают.\n");
    }

    free(arrNext);
    free(arrPrev);
}


