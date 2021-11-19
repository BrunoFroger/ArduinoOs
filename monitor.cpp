//----------------------------------
//
//      monitor.cpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------

#include <arduino.h>
#include "variables.hpp"
#include "task.hpp"
#include "log.hpp"


char command[50];

//-----------------------------------
//
//      monitor_displayPrompt
//
//-----------------------------------
void monitor_displayPrompt(void){
    log("arduinoOS > ");
}


//-----------------------------------
//
//      monitor_init
//
//-----------------------------------
void monitor_init(taskStruct *task){
    //log("monitor_init => debut\n", command);
    strcpy(command,"");
    task->status = WAIT;
}


//-----------------------------------
//
//      monitor_runCommand
//
//-----------------------------------
void monitor_runCommand(taskStruct *task, char *command){
    //log("monitor_runCommand => command = <%s>\n", command); 
    command[strlen(command)]=='\0';
    //log("monitor_runCommand > command = <%s>", command); 
    //log("monitor_runCommand => %s\n", command); 
    task_add(command,standardPriority, task->pid);
    task->status=WAIT;
}

//-----------------------------------
//
//      monitor_exec
//
//-----------------------------------
int monitor_exec(taskStruct *task){
    //log("monitor_exec => debut\n");   
    //int cardispo = Serial.available();
    int carlu;
    while(Serial.available() > 0) // tant qu'il y a des caractères à lire
    {
        carlu = Serial.read(); // on lit le caractère
        //log("monitor_exec => car lu = %c\n",carlu); 
        Serial.write(carlu); // puis on le renvoi à l’expéditeur tel quel
        if (carlu == '\n'){
            monitor_runCommand(task, command);
            strcpy(command,"");
        } else {
            int size = strlen(command);
            command[size++]=carlu;
            command[size++]='\0';
        }

    }
    return TASK_RETURN_OK;
}

//-----------------------------------
//
//      monitor_wait
//
//-----------------------------------
void monitor_wait(taskStruct *task){
    if (Serial.available() > 0){
        //log("monitor_wait => caractere saisi\n");
        //monitor_displayPrompt();
        task->status = RUN;
    }
}

//-----------------------------------
//
//      monitor_wakeup
//
//-----------------------------------
void monitor_wakeup(taskStruct *task){
    task->status = RUN;
}