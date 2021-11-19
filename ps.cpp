//----------------------------------
//
//      ps.cpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------

#include <arduino.h>
#include "variables.hpp"
#include "task.hpp"
#include "tools.hpp"
#include "log.hpp"

int nb_ps_iterations;

//-----------------------------------
//
//      ps_init
//
//-----------------------------------
void ps_init(taskStruct *task){
    task->status = RUN;
    char tmp1[200];
    strcpy(tmp1, task->name);
    tools_string_cut(tmp1,' ',1);
    if (strlen(tmp1) != 0){
        nb_ps_iterations = String(tmp1).toInt();
    } else {
        nb_ps_iterations = -1;
    }
    //log("nb_iterations = <%s>:<%d> \n", tmp1, nb_iterations); 
}

//-----------------------------------
//
//      status2string
//
//-----------------------------------
char *status2string(int status){
    switch(status){
        case INIT : return "INIT";
        case FREE : return "FREE";
        case RUN : return "RUN";
        case WAIT : return "WAIT";
        case DEAD : return "DEAD";
        default : return "???";
    }
}

//-----------------------------------
//
//      ps_exec
//
//-----------------------------------
int ps_exec(taskStruct *task){
    taskStruct ps_task;
    log("+-----+----------------------------------------------------+-------+-------+-----+-----+------+\n"); 
    log("+  id +                                          name      +  pid  +  ppid + prio+ curr+status+\n"); 
    log("+-----+----------------------------------------------------+-------+-------+-----+-----+------+\n"); 
    for (int i = 0 ; i< NB_TASKS ; i++){
        ps_task=taskTbl[i];
        if (ps_task.status != FREE){
            log("+ %3d + %50s + %5d + %5d + %3d + %3d + %4s +\n",i, ps_task.name, ps_task.pid, ps_task.ppid, ps_task.priority, ps_task.currentPriority, status2string(ps_task.status)); 
        }
    }
    log("+-----+----------------------------------------------------+-------+-------+-----+-----+------+\n"); 
    if (--nb_ps_iterations <= 0) task->status = DEAD;
}

//-----------------------------------
//
//      ps_wait
//
//-----------------------------------
void ps_wait(taskStruct *task){
    task->status = DEAD;
}

//-----------------------------------
//
//      ps_wakeup
//
//-----------------------------------
void ps_wakeup(taskStruct *task){
    task->status = DEAD;
}
