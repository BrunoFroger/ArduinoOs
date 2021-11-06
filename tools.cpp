//----------------------------------
//
//      tools.cpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>
#include "variables.hpp"

char arduinoOSTime[50];

//----------------------------------
//
//      tools_string_cut
//
//----------------------------------
char *tools_string_cut(char *inputString, char delimiter, int index){
    int cpt = 0;
    char result[25]="";
    int ptrResult=0;
    for (int i = 0 ; i < strlen(inputString) ; i++){
        //sprintf(printString, "analyse du carctere %c\n",inputString[i]);Serial.print(printString);
        if (inputString[i] == delimiter){
            cpt++;
        }
        if (cpt == index){
            if (inputString[i] != delimiter){
                result[ptrResult++] = inputString[i];
                result[ptrResult] = '\0';
            }
        }
    }
    strcpy(inputString, result);
    return inputString;
}

//----------------------------------
//
//      get_time
//
//----------------------------------
char *get_time(void){
    unsigned long myTime=millis();
    unsigned long tmpTime=myTime;
    ltoa(myTime,arduinoOSTime,10);
    int ms=tmpTime%1000;
    tmpTime -= ms;
    tmpTime /= 1000;
    int sec = tmpTime % 60;
    tmpTime -= sec;
    tmpTime /= 60;
    int mn = tmpTime % 60;
    tmpTime -= mn;
    tmpTime /= 60;
    int hh = tmpTime % 60;

    tmpTime = ( ( ( ( hh * 60 ) + mn * 60) + sec ) * 1000 ) + ms; 
    sprintf(arduinoOSTime, "%02d:%02d:%02d:%03d => ", hh,mn,sec,ms);
    return arduinoOSTime;
}
