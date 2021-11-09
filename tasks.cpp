//----------------------------------
//
//      task.cpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------

#include <arduino.h>
#include <string.h>
#include "variables.hpp"
#include "monitor.hpp"
#include "ps.hpp"
#include "env.hpp"
#include "set.hpp"
#include "kill.hpp"
#include "nop.hpp"
#include "priority.hpp"
#include "afficheur.hpp"
#include "sdcard.hpp"
#include "interpreteur.hpp"

//-----------------------------------
//
//      task_init
//
//-----------------------------------
void task_init(void){
    //sprintf(printString, "task_init => debut\n"); Serial.print(printString);
    int i=0;
    for (i = 0 ; i < NB_TASKS ; i++){
        task_reset(i);
    }
}

//-----------------------------------
//
//      task_add
//
//-----------------------------------
int task_add(char *name, int priority, int parentPid){
    bool tacheNameOK = false;
    int (*execFct)(taskStructPtr);
    void (*initFct)(taskStructPtr);
    void (*waitFct)(taskStructPtr);
    void (*wakeupFct)(taskStructPtr);
    //sprintf(printString, "task_add => debut\n"); Serial.print(printString);
    sprintf(printString, "task_add => tentative ajout tache %s \n", name); Serial.print(printString);
    if (strcmp(name, "") == 0){
        return true;
    } else if (strncmp(name, "monitor", 7) == 0){
        //sprintf(printString, "task_add => monitor\n"); Serial.print(printString);
        tacheNameOK = true;
        execFct=monitor_exec;
        initFct=monitor_init;
        waitFct=monitor_wait;
        wakeupFct=monitor_wakeup;
    } else if (strncmp(name, "ps", 2) == 0){
        //sprintf(printString, "task_add => ps\n"); Serial.print(printString);
        tacheNameOK = true;
        execFct=ps_exec;
        initFct=ps_init;
        waitFct=ps_wait;
        wakeupFct=ps_wakeup;
    } else if (strncmp(name, "env", 3) == 0){
        //sprintf(printString, "task_add => env\n"); Serial.print(printString);
        tacheNameOK = true;
        execFct=env_exec;
        initFct=env_init;
        waitFct=env_wait;
        wakeupFct=env_wakeup;
    } else if (strncmp(name, "set", 3) == 0){
        //sprintf(printString, "task_add => set\n"); Serial.print(printString);
        tacheNameOK = true;
        execFct=set_exec;
        initFct=set_init;
        waitFct=set_wait;
        wakeupFct=set_wakeup;
    } else if (strncmp(name, "kill", 4) == 0){
        //sprintf(printString, "task_add => kill\n"); Serial.print(printString);
        tacheNameOK = true;
        execFct=kill_exec;
        initFct=kill_init;
        waitFct=kill_wait;
        wakeupFct=kill_wakeup;
    } else if (strncmp(name, "nop", 3) == 0){
        //sprintf(printString, "task_add => nop\n"); Serial.print(printString);
        tacheNameOK = true;
        execFct=nop_exec;
        initFct=nop_init;
        waitFct=nop_wait;
        wakeupFct=nop_wakeup;
    } else if (strncmp(name, "priority", 8) == 0){
        //sprintf(printString, "task_add => priority\n"); Serial.print(printString);
        tacheNameOK = true;
        execFct=priority_exec;
        initFct=priority_init;
        waitFct=priority_wait;
        wakeupFct=priority_wakeup;
    } else if (strncmp(name, "afficheur", 8) == 0){
        //sprintf(printString, "task_add => afficheur\n"); Serial.print(printString);
        tacheNameOK = true;
        execFct=afficheur_exec;
        initFct=afficheur_init;
        waitFct=afficheur_wait;
        wakeupFct=afficheur_wakeup;
    } else if (strncmp(name, "sdcard", 6) == 0){
        //sprintf(printString, "task_add => sdcard\n"); Serial.print(printString);
        tacheNameOK = true;
        execFct=sdcard_exec;
        initFct=sdcard_init;
        waitFct=sdcard_wait;
        wakeupFct=sdcard_wakeup;
    } else if (strncmp(name, "interpreteur", 6) == 0){
        //sprintf(printString, "task_add => interpreteur\n"); Serial.print(printString);
        tacheNameOK = true;
        execFct=interpreteur_exec;
        initFct=interpreteur_init;
        waitFct=interpreteur_wait;
        wakeupFct=interpreteur_wakeup;
    } else {
        if (is_sdcard_initialized()){
            char tmp1[100];
            strcpy(tmp1, "sdcard ");
            strcat(tmp1, name);
            strcpy(name, tmp1);
            tacheNameOK = true;
            execFct=sdcard_exec;
            initFct=sdcard_init;
            waitFct=sdcard_wait;
            wakeupFct=sdcard_wakeup;
        }
    }
    if (tacheNameOK){
        for (int i = 0 ; i < NB_TASKS ; i++){
            if (taskTbl[i].status == FREE){
                taskStruct *task = &taskTbl[i];
                if (parentPid != -1){
                    task->ppid=parentPid;
                } else {
                    task->ppid=0;
                }
                strcpy(task->name,name);
                task->pid=newPid++;
                task->priority = priority;
                task->currentPriority=task->priority;
                task->status = INIT;
                task->initFct=initFct;
                task->execFct=execFct;
                task->waitFct=waitFct;
                task->wakeupFct=wakeupFct;
                //sprintf(printString, "task_add => tache %s ajouteee en pos %d\n", task->name, i); Serial.print(printString);
                //sprintf(printString, "task_add => idx parent = %d\n", parentIdx); Serial.print(printString);
                //sprintf(printString, "task_add => ppid       = %d\n", task->ppid); Serial.print(printString);
                //sprintf(printString, "task_add => status = %d\n", task->status); Serial.print(printString);
                return task->pid;
            }
        }
    } 
    sprintf(printString, "task_add => fin\n"); Serial.print(printString);
    return -1;
}

//-----------------------------------
//
//      task_reset
//
//-----------------------------------
void task_reset(int idx){
    taskStruct *task = &taskTbl[idx];
    strcpy(task->name,"");
    task->pid=-1;
    task->ppid=-1;
    task->priority=-1;
    task->currentPriority=-1;
    task->status=FREE;
    task->execFct=nullptr;
}

