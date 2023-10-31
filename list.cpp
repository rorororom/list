#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "log_funcs.h"
#include "list.h"

static void FillArrayFrom (int* prev, int* arrPrev, int i, struct List* list);
static void InitializationArr (int* arr, int size);
static void InitializationPrev (struct List* list);
static void InitializationNext (struct List* list);

static void Verify (struct List* list);
static int VerifyMeaningData (struct List* list);
static void ErrorCodes (int sum_errors);

static void DumpList (struct List* list);
static void Partion (FILE* file, struct List* list);

int main()
{
    OpenLogFile("LOGE.log", "w");

    struct List list = {};

    CtorList(&list);

    DumpList(&list);

    for (int i = 0; i < WANT_COUNT_ADD; i++)
    {
        PushElement(&list, i, 10 * (i+1));
    }

    PopElement(&list, 3);
    PopElement(&list, 5);
    PopElement(&list, 1);
    PushElement(&list, 4, 34);

    Verify(&list);
    GenerateImage(&list);

    DtorList(&list);
}

void CtorList (struct List* list)
{
    list->free = 1;
    list->head = 1;
    list->tail = 1;
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
    list->next[0] = 1;
    list->prev[0] = -1;
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
    Verify(list);
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
    Verify(list);
    DumpList(list);
}

static void InitializationNext (struct List* list)
{
    assert(list != nullptr);

    for (int i = 1; i < SIZE_DATA; i++)
    {
        if (i == SIZE_DATA - 1) list->next[i] = 0;
        else list->next[i] =  -(i+1);
    }
}

static void InitializationPrev (struct List* list)
{
    assert(list != nullptr);

    for (int i = 1; i < SIZE_DATA; i++)
    {
        if (i == 1) list->prev[i] = 0;
        else list->prev[i] = -1;
    }
}

static void InitializationArr (int* arr, int size)
{
    assert(arr != nullptr);

    for (int i = 0; i < size; i++)
    {
        arr[i] = 0;
    }
}

static void DumpList (struct List* list)
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
    fprintf(LOG_FILE, "tail: [%.3d]\n", list->tail);
    fprintf(LOG_FILE, "free: [%.3d]\n", list->free);

}

static void Partion(FILE* file, struct List* list)
{
    assert(list != nullptr);

    fprintf(file, "\n           ");
    for (int i = 0; i < SIZE_DATA - 1; i++)
    {
        fprintf(file, "|_____");
    }
}

static void Verify (struct List* list)
{
    int sum_errors = 0;

    assert(list != NULL);
    VERIFY_VALUE(data, == NULL, DATA);
    VERIFY_VALUE(next, == NULL, NEXT);
    VERIFY_VALUE(prev, == NULL, PREV);
    VERIFY_VALUE(size, < 0, SIZE);
    VERIFY_VALUE(free, <= 0, FREE);
    VERIFY_VALUE(head, <= 0, HEAD);
    VERIFY_VALUE(tail, <= 0, TAIL);

    if (VerifyMeaningData (list))
    {
        fprintf(LOG_FILE, "errror VerifyMeaningData\n");
        sum_errors = sum_errors | ERROR_MEANING_BIT;
    }
    else
    {
        fprintf(LOG_FILE, "\n****************************************\n");
        fprintf(LOG_FILE, "массивы совпадают\n");
        fprintf(LOG_FILE, "****************************************\n");
    }

    if (sum_errors > 0) ErrorCodes(sum_errors);
}

static void ErrorCodes (int sum_errors)
{
    PRINT_ERRORS(size, SIZE);
    PRINT_ERRORS(data, DATA);
    PRINT_ERRORS(next, NEXT);
    PRINT_ERRORS(prev, PREV);
    PRINT_ERRORS(free, FREE);
    PRINT_ERRORS(head, HEAD);
    PRINT_ERRORS(tail, TAIL);
    PRINT_ERRORS(meaning, MEANING);
}

static int VerifyMeaningData (struct List* list)
{
    int* arrNext = (int*)malloc(list->size * sizeof(int));
    CHECK_MALLOC(arrNext);
    int* arrPrev = (int*)malloc(list->size * sizeof(int));
    CHECK_MALLOC(arrPrev);

    InitializationArr(arrNext, list->size);
    InitializationArr(arrPrev, list->size);

    FillArrayFrom(list->next, arrNext, list->head, list);
    FillArrayFrom(list->prev, arrPrev, list->tail, list);

    int count = 0;
    while (count < list->size / 2)
    {
        int temp = arrPrev[count];
        arrPrev[count] = arrPrev[list->size - 1 - count];
        arrPrev[list->size - 1 - count] = temp;
        count++;
    }

    int arraysMatch = 0;
    for (int j = 0; j < list->size; j++)
    {
        if (arrNext[j] != arrPrev[j])
        {
            arraysMatch = 1;
            break;
        }
    }

    if (arraysMatch == 0) return 0;
    else return 1;

    free(arrNext);
    free(arrPrev);
}

static void FillArrayFrom(int* prev, int* arrPrev, int i, struct List* list)
{
    int count = 0;
    while (i >= 0 && count < list->size)
    {
        arrPrev[count] = list->data[i];
        count++;
        i = prev[i];
    }
}

void GenerateImage (struct List* list)
{
    FILE* dotFile = fopen("grath.dot", "w");

    if (dotFile)
    {
        fprintf(dotFile, "digraph G {\n");
        fprintf(dotFile, "rankdir=LR;\n");

        fprintf(dotFile, "subgraph cluster_addresses {\n");
        fprintf(dotFile, "rankdir=TB;\n");
        fprintf(dotFile, "rank=same;\n");

        for (int i = 0; i < SIZE_DATA; i++)
        {
            fprintf(dotFile, "node%d [shape=box, label=\"Address: %d\"];\n", i, i);
        }
        fprintf(dotFile, "}\n");

        fprintf(dotFile, "subgraph cluster_data {\n");
        fprintf(dotFile, "rankdir=LR;\n"); 
        fprintf(dotFile, "rank=same;\n");
        for (int i = 0; i < SIZE_DATA; i++)
        {
            fprintf(dotFile, "nodeA%d [shape=record, label=\"Value: %d | Next: %d | Prev: %d\", rank=same];\n", i, list->data[i], list->next[i], list->prev[i]);
        }
        int cnt = 0;
        for (int i = 0; i < list->size; i++)
        {
            if (list->next[cnt] >= 0)
            {
                fprintf(dotFile, "nodeA%d:next -> nodeA%d;\n", i, list->next[cnt]);
            }
            cnt = list->next[cnt];
        }
        fprintf(dotFile, "}\n");




        for (int i = 0; i < SIZE_DATA; i++)
        {
            fprintf(dotFile, "node%d -> nodeA%d;\n", i, i);

        }

        for (int i = 0; i < SIZE_DATA - 1; i++)
        {
            fprintf(dotFile, "node%d -> node%d;\n", i, i + 1);
        }


        // int cnt = 0;
        // for (int i = 0; i < list->size; i++)
        // {
        //     if (list->next[cnt] >= 0)
        //     {
        //         fprintf(dotFile, "nodeA%d:next -> nodeA%d;\n", i, list->next[cnt]);
        //     }
        //     cnt = list->next[cnt];
        // }

        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }
    else
    {
        fprintf(stderr, "Ошибка при открытии файла grath.dot\n");
    }
}






