#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#include "log_funcs.h"
#include "list.h"

char count_gr[] = "1";
static int cntGraph = 0;

static void FillArrayFrom (int* prev, int* arrPrev, int i, struct List* list);
static void InitializationArr (int* arr, int size);
static void InitializationPrev (struct List* list);
static void InitializationNext (struct List* list);

int GetHead(struct List* list);
int GetTail(struct List* list);

static int VerifyMeaningData (struct List* list);
static void ErrorCodes (int sum_errors);

static void Partion (FILE* file);

static void CreateNode(FILE* dotFile, int index, const char* fillColor, struct List* list);

void CtorList (struct List* list)
{
    list->free = 1;
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

    list->data[0] = ZERO_ELEMENT;
    list->next[0] = 0;
    list->prev[0] = 0;
}

void DtorList (struct List* list)
{
    assert(list != nullptr);
    for (int i = 0; i < SIZE_DATA; i++)
    {
        list->data[i] = ZERO_ELEMENT;
        list->next[i] = ZERO_ELEMENT;
        list->prev[i] = ZERO_ELEMENT;
    }

    free(list->data);
    free(list->next);
    free(list->prev);

    list->free = FREE_ELEMENT;
    list->size = FREE_ELEMENT;
}

void PushElement (struct List* list, int index, int value)
{
    assert(list != nullptr);
    assert(index >= ZERO_ELEMENT && index < SIZE_DATA);

    if (list->data[index] < 0)
    {
        printf("нельзя добавлять после свободного\n");
        fprintf(LOG_FILE, "нельзя добавлять после свободного\n");
        return;
    }

    int nowIndex = abs(list->free);
    list->free = abs(list->next[nowIndex]);
    list->data[nowIndex] = value;

    list->next[nowIndex] = list->next[index];
    list->prev[list->next[nowIndex]] = nowIndex;
    list->next[index] = nowIndex; //следующий элемент индекса, это индекс нового элемента
    list->prev[nowIndex] = index; //предыдущий элемент для нового это индекс

    list->size++;
    Verify(list);
    DumpList(list);
    //CreateNewGraph();
    return;
}

void PopElement (struct List* list, int index)
{
    assert(index > ZERO_ELEMENT && index < SIZE_DATA);
    assert(list != nullptr);

    if (list->data[index] < 0)
    {
        printf("нельзя удалять свободный\n");
        fprintf(LOG_FILE, "нельзя удалять свободный\n");
        return;
    }

    list->prev[list->next[index]] = list->prev[index];
    list->next[list->prev[index]] = list->next[index];

    list->next[index] = -list->free;
    list->prev[index] = list->prev[abs(list->free)];
    list->data[index] = ZERO_ELEMENT;
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
        if (i == SIZE_DATA - 1) list->next[i] = FREE_ELEMENT;
        else list->next[i] =  -(i+1);
    }
}

static void InitializationPrev (struct List* list)
{
    assert(list != nullptr);

    for (int i = 1; i < SIZE_DATA; i++)
    {
        if (i == 1) list->prev[i] = ZERO_ELEMENT;
        else list->prev[i] = FREE_ELEMENT;
    }
}

static void InitializationArr (int* arr, int size)
{
    assert(arr != nullptr);

    for (int i = 0; i < size; i++)
    {
        arr[i] = ZERO_ELEMENT;
    }
}

void DumpList (struct List* list)
{
    assert(list != nullptr);

    fprintf(LOG_FILE, "index: ");
    for (int i = 0; i < SIZE_DATA; i++)
    {
        fprintf(LOG_FILE, "%.3d_|_", i);
    }

    Partion(LOG_FILE);

    fprintf(LOG_FILE, "\ndata: ");
    for (int i = 0; i < SIZE_DATA; i++)
    {
        fprintf(LOG_FILE, " %.3d |", list->data[i]);
    }

    Partion(LOG_FILE);

    fprintf(LOG_FILE, "\nnext: ");
    for (int i = 0; i < SIZE_DATA; i++)
    {
        if (list->next[i] < 0) fprintf(LOG_FILE, "%.3d |", list->next[i]);
        else fprintf(LOG_FILE, " %.3d |", list->next[i]);
    }

    Partion(LOG_FILE);

    fprintf(LOG_FILE, "\nprev: ");
    for (int i = 0; i < SIZE_DATA; i++)
    {
        if (list->prev[i] < ZERO_ELEMENT) fprintf(LOG_FILE, "%.3d |", list->prev[i]);
        else fprintf(LOG_FILE, " %.3d |", list->prev[i]);
    }
    Partion(LOG_FILE);


    fprintf(LOG_FILE, "\n\nhead: [%.3d]\n", GetHead(list));
    fprintf(LOG_FILE, "tail: [%.3d]\n", GetTail(list));
    fprintf(LOG_FILE, "free: [%.3d]\n", list->free);

}

static void Partion(FILE* file)
{
    fprintf(file, "\n           ");
    for (int i = 0; i < SIZE_DATA - 1; i++)
    {
        fprintf(file, "|_____");
    }
}

void Verify (struct List* list)
{
    int sum_errors = 0;

    assert(list != NULL);
    VERIFY_VALUE(data, == NULL, DATA);
    VERIFY_VALUE(next, == NULL, NEXT);
    VERIFY_VALUE(prev, == NULL, PREV);
    VERIFY_VALUE(size, < ZERO_ELEMENT, SIZE);
    VERIFY_VALUE(free, <= ZERO_ELEMENT, FREE);

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

    FillArrayFrom(list->next, arrNext, GetHead(list), list);
    FillArrayFrom(list->prev, arrPrev, GetTail(list), list);

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
    while (i >= ZERO_ELEMENT && count < list->size)
    {
        arrPrev[count] = list->data[i];
        count++;
        i = prev[i];
    }
}

void GenerateImage (struct List* list)
{
    FILE* dotFile = fopen("grapth.dot", "w");

    if (dotFile)
    {
        fprintf(dotFile, "digraph structs {\n");
        fprintf(dotFile, "\trankdir=LR;\n");
        fprintf(dotFile, "\tgraph [bgcolor=\"#fff5ee\"]\n");
        fprintf(dotFile, "\tnode[color=\"#b02f15\",fontsize=14];\n");
        fprintf(dotFile, "slipnes = ortho\n");
        fprintf(dotFile, "\tedge[fontsize=22];\n\n\n");

        for (int i = 0; i < SIZE_DATA; i++)
        {
            if (i == ZERO_ELEMENT)
            {
                CreateNode(dotFile, i, "#d5a1a7", list);
            }
            else if (list->next[i] < ZERO_ELEMENT || (i == SIZE_DATA - 1 && list->next[i] == FREE_ELEMENT))
            {
                CreateNode(dotFile, i, "#6495ed", list);
            }
            else
            {
                CreateNode(dotFile, i, "#bba6cd", list);
            }
        }
        fprintf(dotFile, "\n\t");

        for (int i = 0; i < SIZE_DATA - 1; i++) {
            fprintf(dotFile, "%d->", i);
        }
        fprintf(dotFile, "%d[weight = 1000000, color = \"#fff5ee\"];\n", SIZE_DATA - 1);

        for(int i = 0; i < SIZE_DATA - 1; i++) {
            if (list->next[i] >= ZERO_ELEMENT)
            {
                fprintf(dotFile, "\t%d->%d[color = \"#1f0932\", slipnes = ortho, constraint=false];\n", i, list->next[i]);
            }
        }

        for (int i = 0; i < SIZE_DATA; i++) {
            if (list->prev[i] != FREE_ELEMENT) {
                fprintf(dotFile, "\t %d -> %d[style = dashed, color = \"#997caf\", slipnes = ortho];\n", i, list->prev[i]);
            }
        }

    fprintf(dotFile, "\thead [shape=oval, style=filled, color=\"#310062\", fillcolor=\"#fff0f5\", label=\"HEAD\" ];\n");
    fprintf(dotFile, "\ttail [shape=oval, style=filled, color=\"#310062\", fillcolor=\"#fff0f5\", label=\"TAIL\" ];\n");
    fprintf(dotFile, "\tfree [shape=oval, style=filled, color=\"#310062\", fillcolor=\"#fff0f5\", label=\"FREE\" ];\n");
    fprintf(dotFile, "\thead->%d[color = \"#f1829d\"];\n", GetHead(list));
    fprintf(dotFile, "\ttail->%d[color = \"#f1829d\"];\n", GetTail(list));
    fprintf(dotFile, "\tfree->%d[color = \"#f1829d\"];\n", list->free);
    fprintf(dotFile, "\n}");
        fclose(dotFile);
    }
    else
    {
        fprintf(stderr, "Ошибка при открытии файла grath.dot\n");
    }
}

static void CreateNode(FILE* dotFile, int index, const char* fillColor, struct List* list)
{
    fprintf(dotFile, "%d [shape=record, style=\"filled,rounded\",\
                            color=\"#552d7b\", fillcolor=\"%s\",\
                            fontsize=14, label=\"\
                                                ip:   %d |\
                                                data: %d |\
                                                next: %d |\
                                                prev: %d\" ];\n",
            index, fillColor, index, list->data[index], list->next[index], list->prev[index]);
}

static int imageCounter = 0;

void CreateNewGraph()
{
    char filename[100];
    sprintf(filename, "grath_%04d.png", imageCounter);

    char command[1000];
    sprintf(command, "dot -Tpng /Users/aleksandr/Desktop/list/grapth.dot -o /Users/aleksandr/Desktop/list/%s", filename);
    system(command);

    imageCounter++;
}

int GetHead(struct List* list)
{
    return list->next[0];
}

int GetTail(struct List* list)
{
    return list->prev[0];
}








