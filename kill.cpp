//----------------------------------
//
//      kill.cpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------

#include <arduino.h>
#include "variables.hpp"
#include "task.hpp"
#include "kill.hpp"
#include "tools.hpp"

//-----------------------------------
//
//      kill_init
//
//-----------------------------------
void kill_init(taskStruct *task){
    task->status = RUN;
}

//-----------------------------------
//
//      kill_exec
//
//-----------------------------------
int kill_exec(taskStruct *task){
    //sprintf(printString, "kill_exec => debut\n"); Serial.print(printString);    
    //sprintf(printString, "kill_exec => commande = %s\n",task->name); Serial.print(printString);
    char strPid[25];
    strcpy(strPid,task->name);
    tools_string_cut(strPid,' ', 1);
    if (strlen(strPid) <= 0){
        //sprintf(printString, "kill => erreur pid manquant\n"); Serial.print(printString);
        task->status = DEAD;
        return 0;
    }
    //sprintf(printString, "kill_exec => pid = %s\n",strPid); Serial.print(printString);
    int pid = String(strPid).toInt();
    //sprintf(printString, "kill_exec => pid = %d\n",pid); Serial.print(printString);
    for (int i = 0 ; i < NB_TASKS ; i++){
        taskStruct *tache = &taskTbl[i];
        if (pid == tache->pid){
            task_reset(i);
        }
    }
    task->status = DEAD;
}

//-----------------------------------
//
//      kill_wait
//
//-----------------------------------
void kill_wait(taskStruct *task){
    task->status = DEAD;
}

//-----------------------------------
//
//      kill_wakeup
//
//-----------------------------------
void kill_wakeup(taskStruct *task){
    task->status = DEAD;
}