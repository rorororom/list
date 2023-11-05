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

static void Verify (struct List* list);
static int VerifyMeaningData (struct List* list);
static void ErrorCodes (int sum_errors);

static void ListElemCtor(Node** elem);
static void ListElemInit(Node* elem, int value, Node* prev, Node* next);
static void ListElemDtor(Node* elem);
Node* GetHead(List* list);
Node* GetTail(List* list);

void CtorList(struct List* list)
{
    assert(list);

    Node* zeroElement = nullptr;
    ListElemCtor(&zeroElement);

    list->end = zeroElement;
    list->size = 0;
}

static void ListElemCtor(Node** elem)
{
    assert(elem);

    *elem = (Node*)calloc(1, sizeof(**elem));

    if (elem == nullptr)
        printf("error memory limit\n");

    (*elem)->value = ZERO_ELEMENT;
    (*elem)->next  = *elem;
    (*elem)->prev  = *elem;
}

void DtorList(List* list)
{
    assert(list);

    Node* listHead = GetHead(list);
    Node* listTail  = GetTail(list);

    while (listHead != listTail)
    {
        Node* NNNode = listHead;

        listHead = listHead->next;

        ListElemDtor(NNNode);
    }

    ListElemDtor(list->end);
    list->end = nullptr;
}

void PushElement(List* list, Node* armature, int value, Node** insertedValPtr)
{
    assert(list);
    assert(armature);
    assert(insertedValPtr);

    Node* newNode = nullptr;
    ListElemCtor(&newNode);

    *insertedValPtr = newNode;

    ListElemInit(newNode, value, armature, armature->next);
    armature->next->prev = newNode;
    armature->next       = newNode;

    list->size++;
    DumpList(list);

    GenerateImage(list);
    CreateNewGraph();
}


static void ListElemInit(Node* elem, int value, Node* prev, Node* next)
{
    assert(elem);

    elem->value = value;
    elem->prev  = prev;
    elem->next  = next;
}

void PopElement(List* list, Node* armature)
{
    assert(list);
    assert(armature);

    armature->prev->next = armature->next;
    armature->next->prev = armature->prev;

    ListElemInit(armature, ZERO_ELEMENT, nullptr, nullptr);

    list->size--;

    DumpList(list);

    GenerateImage(list);
    CreateNewGraph();
}

static void ListElemDtor(Node* elem)
{
    assert(elem);

    elem->next  = elem->prev = nullptr;
    elem->value = ZERO_ELEMENT;

    free(elem);
}

Node* GetHead(List* list)
{
    assert(list);
    assert(list->end);

    return list->end->next;
}

Node* GetTail(List* list)
{
    assert(list);
    assert(list->end);

    return list->end->prev;
}

void DumpList (struct List* list)
{
    assert(list);

    fprintf(LOG_FILE, "<table border='1'>\n");

    int i = 0;
    fprintf(LOG_FILE, "\t<tr>\n");
    fprintf(LOG_FILE, "\t\t<td>index</td>\n", i);
    for (Node* listElem = GetHead(list); listElem != list->end; listElem = listElem->next)
    {
        fprintf(LOG_FILE, "\t\t<td>%d</td>\n", i);
        i++;
    }
    fprintf(LOG_FILE, "\t</tr>\n");

    fprintf(LOG_FILE, "\t<tr>\n");
    fprintf(LOG_FILE, "\t\t<td>ip</td>\n", i);
    for (Node* listElem = GetHead(list); listElem != list->end; listElem = listElem->next)
    {
        fprintf(LOG_FILE, "\t\t<td>%p</td>\n", listElem);
    }
    fprintf(LOG_FILE, "\t</tr>\n");

    fprintf(LOG_FILE, "\t<tr>\n");
    fprintf(LOG_FILE, "\t\t<th>data</th>\n");
    for (Node* listElem = GetHead(list); listElem != list->end; listElem = listElem->next)
    {
        fprintf(LOG_FILE, "\t\t<td>%d</td>\n", listElem->value);
    }
    fprintf(LOG_FILE, "\t</tr>\n");

    fprintf(LOG_FILE, "\t<tr>\n");
    fprintf(LOG_FILE, "\t\t<th>next</th>\n");
    for (Node* listElem = GetHead(list); listElem != list->end; listElem = listElem->next)
    {
        fprintf(LOG_FILE, "\t\t<td>%p</td>\n", listElem->next);
    }
    fprintf(LOG_FILE, "\t</tr>\n");

    fprintf(LOG_FILE, "\t<tr>\n");
    fprintf(LOG_FILE, "\t\t<th>prev</th>\n");
    for (Node* listElem = GetHead(list); listElem != list->end; listElem = listElem->next)
    {
        fprintf(LOG_FILE, "\t\t<td>%p</td>\n", listElem->prev);
    }
    fprintf(LOG_FILE, "\t</tr>\n");

    fprintf(LOG_FILE, "</table>\n");
}

void Partion(FILE* file, struct List* list)
{
    assert(list != nullptr);

    fprintf(file, "<br>\n");
    for (int i = 0; i < SIZE_DATA - 1; i++)
    {
        fprintf(file, "|________________");
    }
    fprintf(file, "<br>\n");
}

static void Verify (struct List* list)
{
    int sum_errors = 0;

    assert(list != NULL);
    if (list->size > SIZE_DATA)
    {
        sum_errors = sum_errors | ERROR_SIZE_BIT;
    }

    if (VerifyMeaningData(list))
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
    PRINT_ERRORS(meaning, MEANING);
}

int VerifyMeaningData(struct List* list)
{
    Node* currentNode = GetHead(list);
    Node* tailNode = GetTail(list);

    while (currentNode != NULL && tailNode != NULL)
    {
        if (currentNode->value != tailNode->value)
        {
            return 1; // Данные не соответствуют ожиданиям
        }
        currentNode = currentNode->next;
        tailNode = tailNode->prev;
    }

    return 0; // Данные в порядке
}

void GenerateImage (struct List* list)
{
    FILE* dotFile = fopen("grapth.dot", "w");

    Node* elem = list->end;
    int elem_amt = list->size + 1;

    if (dotFile)
    {
        fprintf(dotFile, "digraph structs {\n");
        fprintf(dotFile, "\trankdir=LR;\n");
        fprintf(dotFile, "\tgraph [bgcolor=\"#fff5ee\"]\n");
        fprintf(dotFile, "\tnode[color=\"#b02f15\",fontsize=14];\n");
        fprintf(dotFile, "splines = ortho;\n");

        fprintf(dotFile, "\tedge[fontsize=22];\n\n\n");

        CreateNode(dotFile, list->end, list->end->value, list->end->next, list->end->prev);
        fprintf(dotFile, "%d -> %d[weight = 1000000, color = \"#fff5ee\"];\n", list->end, list->end->next);

        Node* listTail = GetTail(list);
        for (Node* listElem =GetHead(list); listElem != listTail; listElem = listElem->next)
        {
            CreateNode(dotFile, listElem, listElem->value, listElem->next, listElem->prev);
            fprintf(dotFile, "%d -> %d[weight = 1000000, color = \"#fff5ee\"];\n", listElem, listElem->next);
        }

        CreateNode(dotFile, listTail, listTail->value, listTail->next, listTail->prev);

        Node* nownow = list->end;
        for(int i = 0; i < list->size + 1; i++)
        {
            fprintf(dotFile, "\t%d->%d[color = \"#1f0932\", slipnes = ortho];\n", nownow, nownow->next);
            nownow = nownow->next;
        }

        nownow = list->end;
        for(int i = 0; i < list->size + 1; i++)
        {
            fprintf(dotFile, "\t%d->%d[color = \"#997caf\", slipnes = ortho];\n", nownow, nownow->prev);
            nownow = nownow->prev;
        }


        Node* Head = GetHead(list);
        Node* Tail = GetTail(list);

        fprintf(dotFile, "\thead [shape=oval, style=filled, color=\"#310062\", fillcolor=\"#fff0f5\", label=\"HEAD\" ];\n");
        fprintf(dotFile, "\ttail [shape=oval, style=filled, color=\"#310062\", fillcolor=\"#fff0f5\", label=\"TAIL\" ];\n");
        fprintf(dotFile, "\thead->%d[color = \"#f1829d\"];\n", Head);
        fprintf(dotFile, "\ttail->%d[color = \"#f1829d\"];\n", Tail);
        fprintf(dotFile, "\n}");
        fclose(dotFile);
    }
    else
    {
        fprintf(stderr, "Ошибка при открытии файла grath.dot\n");
    }
}

void CreateNode(FILE* dotFile, Node* id, const int value,
                               Node* next, Node* prev)
{
    fprintf(dotFile, "%d"
                        "[shape=Mrecord, style=filled, fillcolor=\"#bba6cd\","
                        "label=\" id: %p |"
                              "value: %d  |"
                          "<f0> next: %p |"
                          "<f1> prev: %p \","
                        "color = \"#552d7b\"];\n",
                        id, id,
                        value,
                        next, prev);
}

static int imageCounter = 0;

void CreateNewGraph()
{
    char filename[100];
    sprintf(filename, "grath_%04d.png", imageCounter);

    char command[1000];
    sprintf(command, "dot -Tpng /Users/aleksandr/Desktop/list/grapth.dot -o /Users/aleksandr/Desktop/list/grapth/%s", filename);
    system(command);

    char AAAA[100] = "/Users/aleksandr/Desktop/list/grapth/";

    imageCounter++;

    fprintf(LOG_FILE, "<img src=\"%s%s\" alt=\"Graph Image\"> \n", AAAA, filename);
}
