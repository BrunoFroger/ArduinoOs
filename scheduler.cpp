//----------------------------------
//
//      scheduler.cpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>
#include "variables.hpp"
#include "task.hpp"
#include "monitor.hpp"
#include "tools.hpp"

int wakeupPpid;

//-----------------------------------
//
//      scheduler_init
//
//-----------------------------------
void scheduler_init(void){
    currentPriority=standardPriority;
}

//-----------------------------------
//
//      scheduler_loop
//
//-----------------------------------
void scheduler_loop(void){
    int lower_priority=0;
    //sprintf(printString, "scheduler_loop => %s ---------------------------------\n", get_time()); Serial.print(tmp);
    int current_nb_process=0;
    int current_run_process=0;
    int current_wait_process=0;
    int current_active_process=0;
    for (int i=0 ; i < NB_TASKS ; i++){
        switch(taskTbl[i].status){
            case RUN:
                current_nb_process++;
                //sprintf(printString, "scheduler_loop => test tache pos %d => status = %d => RUN tache %s\n",i, taskTbl[i].status,taskTbl[i].name); Serial.print(tmp);
                if (taskTbl[i].currentPriority < currentPriority){
                    taskTbl[i].currentPriority++;
                    current_active_process++;
                } else {
                    taskTbl[i].execFct(&taskTbl[i]);
                    taskTbl[i].currentPriority = taskTbl[i].priority;
                    current_run_process++;
                    if (lower_priority < taskTbl[i].currentPriority){
                        lower_priority=taskTbl[i].currentPriority;
                    }
                }
                break;
            case INIT:
                current_nb_process++;
                //sprintf(printString, "scheduler_loop => test tache pos %d => status = %d => INIT tache %s\n",i, taskTbl[i].status,taskTbl[i].name); Serial.print(printString);
                taskTbl[i].initFct(&taskTbl[i]);
                break;
            case FREE:
                break;
            case DEAD:
                //sprintf(printString, "scheduler_loop => test tache pos %d => status = %d => DEAD tache %s\n",i, taskTbl[i].status,taskTbl[i].name); Serial.print(printString);
                current_nb_process++;
                wakeupPpid=taskTbl[i].ppid;
                for (int j = 0 ; j < NB_TASKS ; j++){
                    if (taskTbl[j].pid == wakeupPpid){
                        //sprintf(printString, "<%s(pid=%d)> reveille sa tache parent <%s(pid=%d)>\n", taskTbl[i].name, taskTbl[i].pid, taskTbl[j].name, wakeupPpid); Serial.print(printString);
                        taskTbl[j].wakeupFct(&taskTbl[j]);
                        break;
                    }
                }
                task_reset(i);
                break;
            case WAIT:
                current_nb_process++;
                //sprintf(printString, "scheduler_loop => test tache pos %d => status = %d => WAIT tache %s\n",i, taskTbl[i].status,taskTbl[i].name); Serial.print(printString);
                taskTbl[i].waitFct(&taskTbl[i]);
                current_wait_process++;
                break;
            default:
                sprintf(printString, "scheduler_loop => status de tache inconnu : %d\n",taskTbl[i].status); Serial.print(printString);
        }
    }
    nb_process=current_nb_process;
    wait_process=current_wait_process;
    wait_process=current_active_process;
    run_process=current_run_process;
    if (run_process <= 0){
        if (currentPriority <= 0){
            currentPriority=0;
        }
    } else {
        currentPriority=lower_priority;
    }
}