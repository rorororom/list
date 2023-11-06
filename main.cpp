#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#include "log_funcs.h"
#include "list.h"

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
    PopElement(&list, 2);
    PushElement(&list, 1, 18);
    PushElement(&list, 2, 25);
    PushElement(&list, 4, 41);
    PopElement(&list, 4);
    PushElement(&list, 3, 25);
    PushElement(&list, 10, 25);


    Verify(&list);
    GenerateImage(&list);

    DtorList(&list);
}
