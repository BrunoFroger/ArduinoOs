//----------------------------------
//
//      log.cpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------



#include <arduino.h>
#include <string.h>
#include <stdarg.h>

char logTabs[25];
int nbTabs=0;

//-----------------------------------
//
//      log
//
//-----------------------------------
void log(const char *format, ...){

    strcpy(logTabs,"");
    for (int i = 0 ; i < nbTabs ; i++) {
        strcat(logTabs, "\t");
    }

    va_list args;
    va_start(args, format);
    printf("%s", logTabs);
    vprintf(format, args);
    va_end(args);
}
