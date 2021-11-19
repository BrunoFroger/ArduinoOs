//----------------------------------
//
//      nop.cpp
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
//      nop_init
//
//-----------------------------------
void nop_init(taskStruct *task){
    //log("nop_init => pid(%d)\n", task->pid); Serial.print(printString);
    task->status = RUN;
}

//-----------------------------------
//
//      nop_exec
//
//-----------------------------------
int nop_exec(taskStruct *task){
    //log("nop_exec => pid(%d)\n", task->pid); Serial.print(printString);
}

//-----------------------------------
//
//      nop_wait
//
//-----------------------------------
void nop_wait(taskStruct *task){
    task->status = DEAD;
}

//-----------------------------------
//
//      nop_wakeup
//
//-----------------------------------
void nop_wakeup(taskStruct *task){
    task->status = RUN;
}