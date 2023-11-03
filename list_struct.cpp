#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#include "log_funcs.h"
#include "list_struct.h"

char count_gr[] = "1";
static int cntGraph = 0;

static void Partion(FILE* file, struct List* list);
static void DumpList (struct List* list);

static int GetHead(struct List* list);
static int GetTail(struct List* list);

static void Verify (struct List* list);
static int VerifyMeaningData (struct List* list);
static void ErrorCodes (int sum_errors);

static void FillArrayFromNext(int* arrPrev, int i, struct List* list);
static void FillArrayFromPrev(int* arrPrev, int i, struct List* list);

static void DumpList (struct List* list);
static void Partion (FILE* file, struct List* list);

static void CreateNode(FILE* dotFile, int index, const char* fillColor, struct List* list);
static void CreateNewGraph();

int main()
{
    OpenLogFile("LOGE.log", "w");

    struct List list = {};
    struct Node node = {};

    CtorList(&list);

    DumpList(&list);

    for (int i = 0; i < WANT_COUNT_ADD; i++)
    {
        PushElement(&list, i, 10 * (i+1));
    }

    PopElement(&list, 3);

    DtorList(&list);
}


void CtorList(struct List* list)
{
    assert(list != NULL);
    list->free = 1;
    list->size = 0;

    list->data[0].prev = 0;
    list->data[0].data = 0;
    list->data[0].next = 0;

    for (int i = 1; i < SIZE_DATA; i++) {
        if (i == SIZE_DATA - 1)
            list->data[i].next = FREE_ELEMENT;
        else
            list->data[i].next = -(i + 1);
    }
}

void DtorList (struct List* list)
{
    assert(list != nullptr);
    for (int i = 0; i < SIZE_DATA; i++)
    {
        list->data[i].data = ZERO_ELEMENT;
        list->data[i].next = ZERO_ELEMENT;
        list->data[i].prev = ZERO_ELEMENT;
    }

    list->free = FREE_ELEMENT;
    list->size = FREE_ELEMENT;
}

void PushElement (struct List* list, int index, int value)
{
    assert(list != nullptr);
    assert(index >= ZERO_ELEMENT && index < SIZE_DATA);

    int nowIndex = abs(list->free);
    list->free = abs(list->data[nowIndex].next);
    list->data[nowIndex].data = value;

    list->data[nowIndex].next = list->data[index].next;
    list->data[list->data[nowIndex].next].prev = nowIndex;
    list->data[index].next = nowIndex;
    list->data[nowIndex].prev = index;

    list->size++;
    //Verify(list);
    DumpList(list);
    //CreateNewGraph();
}

void PopElement (struct List* list, int index)
{
    assert(index >= ZERO_ELEMENT && index < SIZE_DATA);
    assert(list != nullptr);

    list->data[list->data[index].next].prev = list->data[index].prev;
    list->data[list->data[index].prev].next = list->data[index].next;

    list->data[index].next = -list->free;
    list->data[index].prev = list->data[abs(list->free)].prev;
    list->data[index].data = ZERO_ELEMENT;
    list->free = index;

    list->size--;
    //Verify(list);
    DumpList(list);
}

static int GetHead(struct List* list)
{
    return list->data[0].next;
}

static int GetTail(struct List* list)
{
    return list->data[0].prev;
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
        fprintf(LOG_FILE, " %.3d |", list->data[i].data);
    }

    Partion(LOG_FILE, list);

    fprintf(LOG_FILE, "\nnext: ");
    for (int i = 0; i < SIZE_DATA; i++)
    {
        if (list->data[i].next < 0) fprintf(LOG_FILE, "%.3d |", list->data[i].next);
        else fprintf(LOG_FILE, " %.3d |", list->data[i].next);
    }

    Partion(LOG_FILE, list);

    fprintf(LOG_FILE, "\nprev: ");
    for (int i = 0; i < SIZE_DATA; i++)
    {
        if (list->data[i].prev < ZERO_ELEMENT) fprintf(LOG_FILE, "%.3d |", list->data[i].prev);
        else fprintf(LOG_FILE, " %.3d |", list->data[i].prev);
    }
    Partion(LOG_FILE, list);


    fprintf(LOG_FILE, "\n\nhead: [%.3d]\n", GetHead(list));
    fprintf(LOG_FILE, "tail: [%.3d]\n", GetTail(list));
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

    FillArrayFromNext(arrNext, GetHead(list), list);
    FillArrayFromPrev(arrPrev, GetTail(list), list);

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

static void FillArrayFromNext(int* arrPrev, int i, struct List* list)
{
    int count = 0;
    while (i >= ZERO_ELEMENT && count < list->size)
    {
        arrPrev[count] = list->data[i].data;
        count++;
        i = list->data[i].next;
    }
}

static void FillArrayFromPrev(int* arrPrev, int i, struct List* list)
{
    int count = 0;
    while (i >= ZERO_ELEMENT && count < list->size)
    {
        arrPrev[count] = list->data[i].data;
        count++;
        i = list->data[i].prev;
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
            else if (list->data[i].next < ZERO_ELEMENT || (i == SIZE_DATA - 1 && list->data[i].next == FREE_ELEMENT))
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
            if (list->data[i].next >= ZERO_ELEMENT)
            {
                fprintf(dotFile, "\t%d->%d[color = \"#1f0932\", slipnes = ortho, constraint=false];\n", i, list->data[i].next);
            }
        }

        for (int i = 0; i < SIZE_DATA; i++) {
            if (list->data[i].prev != FREE_ELEMENT) {
                fprintf(dotFile, "\t %d -> %d[style = dashed, color = \"#997caf\", slipnes = ortho];\n", i, list->data[i].prev);
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
            index, fillColor, index, list->data[index].data, list->data[index].next, list->data[index].prev);
}

// void CreateNewGraph()
// {
//     time_t now = time(NULL);
//     struct tm* timeinfo = localtime(&now);
//
//     char filename[100];
//     sprintf(filename, "grath_%04d-%02d-%02d_%02d-%02d-%02d.png",
//             timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
//             timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
//
//     char command[1000];
//     sprintf(command, "dot -Tpng /Users/aleksandr/Desktop/list/grapth.dot -o /Users/aleksandr/Desktop/list/grapths/%s", filename);
//     system(command);
// }

