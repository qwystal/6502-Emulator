#include "../emulator/includes.h"

int errors;

void _log(char *text, int errorlevel){
    if (!errorlevel)
    {
        printf("\t\033[32m[+]\033[0m %s\n", text);
    }
    else if (errorlevel)
    {
        printf("\t\033[31m[-]\033[0m %s\n", text);
        errors += 1;
    }
}