//----------------------------------
//
//      set.cpp
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
#include "log.hpp"


//-----------------------------------
//
//      set_init
//
//-----------------------------------
void set_init(taskStruct *task){
    task->status = RUN;
}

//-----------------------------------
//
//      set_exec
//
//-----------------------------------
int set_exec(taskStruct *task){
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
    log("set_exec => key = <%s>, value = <%s>\n", key, value);
    for (int i = 0 ; i < NB_ENV_VARS ; i++){
        if (strcmp(envTbl[i].key,"") == 0){
            strcpy(envTbl[i].key,key);
            strcpy(envTbl[i].value,value);
            setOK=true;
            break;
        }
    }
    if (!setOK){
        log("set => impossible de memoriser cette variable (table pleine)\n", key, value);
    }
    task->status = WAIT;
}

//-----------------------------------
//
//      set_wait
//
//-----------------------------------
void set_wait(taskStruct *task){
    task->status = DEAD;
}

//-----------------------------------
//
//      set_wakeup
//
//-----------------------------------
void set_wakeup(taskStruct *task){
    task->status = DEAD;
}