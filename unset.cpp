//----------------------------------
//
//      unset.cpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------

#include <arduino.h>
#include "variables.hpp"
#include "task.hpp"
#include "env.hpp"
#include "tools.hpp"


//-----------------------------------
//
//      unset_init
//
//-----------------------------------
void unset_init(taskStruct *task){
    task->status = RUN;
}

//-----------------------------------
//
//      unset_exec
//
//-----------------------------------
int unset_exec(taskStruct *task){
    char tmp1[100];
    char key[50];
    char value[50];
    bool setOK=false;
    strcpy(tmp1, task->name);
    tools_string_cut(tmp1,' ', 1);
    strcpy(key, tmp1);
    strcpy(tmp1, task->name);
    tools_string_cut(tmp1,' ', 2);
    strcpy(value, tmp1);
    //sprintf(printString, "set_exec => key = <%s>, value = <%s>\n", key, value); Serial.print(printString);
    for (int i = 0 ; i < NB_ENV_VARS ; i++){
        if (strcmp(envTbl[i].key,"") == 0){
            strcpy(envTbl[i].key,"");
            strcpy(envTbl[i].value,"");
            setOK=true;
            break;
        }
    }
    if (!setOK){
        sprintf(printString, "set => impossible de memoriser cette variable (table pleine)\n", key, value); Serial.print(printString);
    }
    task->status = WAIT;
}


//-----------------------------------
//
//      unset_wait
//
//-----------------------------------
void unset_wait(taskStruct *task){
    task->status = DEAD;
}