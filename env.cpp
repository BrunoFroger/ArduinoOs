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
    //sprintf(printString, "nb_iterations = <%s>:<%d> \n", tmp1, nb_iterations); Serial.print(printString);
}

//-----------------------------------
//
//      env_exec
//
//-----------------------------------
int env_exec(taskStruct *task){
    //sprintf(printString, "env_exec => debut\n"); Serial.print(printString);

    // affichage des variables systeme
    sprintf(printString, "newPid = %d\n", newPid); Serial.print(printString);
    sprintf(printString, "standardPriority = %d\n", standardPriority); Serial.print(printString);
    sprintf(printString, "currentPriority = %d\n", currentPriority); Serial.print(printString);
    sprintf(printString, "nb_process = %d\n", nb_process); Serial.print(printString);
    sprintf(printString, "run_process = %d\n", run_process); Serial.print(printString);
    sprintf(printString, "wait_process = %d\n", wait_process); Serial.print(printString);
    sprintf(printString, "active_process = %d\n", active_process); Serial.print(printString);

    // affichage des variables definies par l'utilisateurs
    for (int i = 0 ; i < NB_ENV_VARS ; i++){
        if (strcmp(envTbl[i].key,"") != 0){
            sprintf(printString, "%s = %s\n", envTbl[i].key, envTbl[i].value); Serial.print(printString);
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