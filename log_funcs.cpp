#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log_funcs.h"

FILE* LOG_FILE = stderr;

void CloseLogFile ()
{
    fprintf (LOG_FILE, "FILE CLOSED\n");
    fprintf (LOG_FILE, "\\----------------------------------------------------\\\n");
    fclose (LOG_FILE);
}

void OpenLogFile (const char* FILE_NAME, const char* mode)
{
    char* file_name = strdup (FILE_NAME);

    LOG_FILE = fopen (file_name, mode);

    if (LOG_FILE == NULL)
    {
        LOG_FILE = stderr;
    }

    fprintf (LOG_FILE, "\\----------------------------------------------------\\\n");
    fprintf (LOG_FILE, "FILE OPEN\n");
    fprintf (LOG_FILE, "\nTime is %s\n", __TIME__);
    atexit (CloseLogFile);
}
