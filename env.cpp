//----------------------------------
//
//      env.cpp
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


envStruct envTbl[NB_ENV_VARS];
int nb_env_iterations;

//-----------------------------------
//
//      env_init
//
//-----------------------------------
void env_init(taskStruct *task){
    task->status = RUN;
    char tmp1[200];
    strcpy(tmp1, task->name);
    tools_string_cut(tmp1,' ',1);
    if (strlen(tmp1) != 0){
        nb_env_iterations = String(tmp1).toInt();
    } else {
        nb_env_iterations = -1;
    }
    //log("nb_iterations = <%s>:<%d> \n", tmp1, nb_iterations); Serial.print(printString);
}

//-----------------------------------
//
//      env_exec
//
//-----------------------------------
int env_exec(taskStruct *task){
    //log("env_exec => debut\n");

    // affichage des variables systeme
    log("newPid = %d\n", newPid);
    log("standardPriority = %d\n", standardPriority);
    log("currentPriority = %d\n", currentPriority);
    log("nb_process = %d\n", nb_process);
    log("run_process = %d\n", run_process);
    log("wait_process = %d\n", wait_process);
    log("active_process = %d\n", active_process);

    // affichage des variables definies par l'utilisateurs
    for (int i = 0 ; i < NB_ENV_VARS ; i++){
        if (strcmp(envTbl[i].key,"") != 0){
            log("%s = %s\n", envTbl[i].key, envTbl[i].value);
        }
    }
    if (--nb_env_iterations <= 0) task->status = DEAD;
}


//-----------------------------------
//
//      env_wait
//
//-----------------------------------
void env_wait(taskStruct *task){
    task->status = DEAD;
}

//-----------------------------------
//
//      env_wakeup
//
//-----------------------------------
void env_wakeup(taskStruct *task){
    task->status = DEAD;
}