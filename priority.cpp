//----------------------------------
//
//      priority.cpp
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
//      priority_init
//
//-----------------------------------
void priority_init(taskStruct *task){
    char tmp1[100];
    task->status = RUN;
    strcpy(tmp1, task->name);
    tools_string_cut(tmp1,' ',1);
    if (strlen(tmp1) != 0){
        standardPriority = String(tmp1).toInt();
    }
}

//-----------------------------------
//
//      priority_exec
//
//-----------------------------------
int priority_exec(taskStruct *task){
    // nothing to do all in init
    task->status = DEAD;
}


//-----------------------------------
//
//      priority_wait
//
//-----------------------------------
void priority_wait(taskStruct *task){
    task->status = DEAD;
}

//-----------------------------------
//
//      priority_wakeup
//
//-----------------------------------
void priority_wakeup(taskStruct *task){
    task->status = DEAD;
}