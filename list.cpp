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

    PushElement(&list, 0, 16);
    PushElement(&list, 1, 20);
    PushElement(&list, 2, 35);
    PushElement(&list, 3, 41);
    PushElement(&list, 4, 52);
    PopElement(&list, 1);
    PushElement(&list, 5, 53);
    PopElement(&list, 1);
    PushElement(&list, 4, 11);
    PushElement(&list, 1, 82);
    PopElement(&list, 1);
    PushElement(&list, 2, 99);


    DtorList(&list);
}

void CtorList (struct List* list)
{
    list->free = 1;
    list->head = 0;
    list->tail = 0;

    list->data = (int*)calloc(SIZE_DATA, sizeof(int));
    list->next = (int*)calloc(SIZE_DATA, sizeof(int));
    list->prev = (int*)calloc(SIZE_DATA, sizeof(int));

    list->data[0] = 0;
    list->next[0] = 0;
    list->prev[0] = 0;
}

void DtorList (struct List* list)
{
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
}

void PushElement (struct List* list, int index, int value)
{
    if (index == 0)
    {
        AddingElementAfter0 (list, value);
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

    DumpList(list);
    VerifyList(list);
    return;
}

void PopElement (struct List* list, int index)
{
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

    DumpList(list);
    VerifyList(list);
}

void InitializationNext (struct List* list)
{
    for (int i = 1; i < SIZE_DATA; i++)
    {
        if (i == SIZE_DATA - 1) list->next[i] = 0;
        else list->next[i] =  -(i+1);
    }
}

void InitializationPrev (struct List* list)
{
    for (int i = 1; i < SIZE_DATA; i++)
    {
        if (i == 1) list->prev[i] = 0;
        else list->prev[i] = -1;
    }
}

void MyIniz (struct List* list)
{
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
    fprintf(file, "\n           ");
    for (int i = 0; i < SIZE_DATA - 1; i++)
    {
        fprintf(file, "|_____");
    }
}

void AddingElementAfter0 (struct List* list, int value)
{
    list->data[1] = value;
    list->next[1] = 0;
    list->prev[1] = 0;

    list->head = 1;
    list->tail = 1;
    list->free = 2;

    DumpList(list);
}

void VerifyList (struct List* list)
{
    int i = list->head;
    while (i != list->tail)
    {
        printf ("i = %d\n", i);
        i = list->next[i];
    }
    printf("-------------------------\n");
}
