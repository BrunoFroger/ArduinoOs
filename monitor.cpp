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


char command[50];

//-----------------------------------
//
//      monitor_displayPrompt
//
//-----------------------------------
void monitor_displayPrompt(void){
    sprintf(printString, "arduinoOS > "); Serial.print(printString);
}


//-----------------------------------
//
//      monitor_init
//
//-----------------------------------
void monitor_init(taskStruct *task){
    sprintf(printString, "monitor_init => debut\n", command); Serial.print(printString);
    strcpy(command,"");
    task->status = WAIT;
}


//-----------------------------------
//
//      monitor_runCommand
//
//-----------------------------------
void monitor_runCommand(taskStruct *task, char *command){
    //sprintf(printString, "monitor_runCommand => command = <%s>\n", command); Serial.print(printString);
    command[strlen(command)]=='\0';
    //sprintf(printString, "monitor_runCommand > command = <%s>", command); Serial.print(printString);
    //sprintf(printString, "monitor_runCommand => %s\n", command); Serial.print(printString);
    task_add(command,standardPriority, task->pid);
    task->status=WAIT;
}

//-----------------------------------
//
//      monitor_exec
//
//-----------------------------------
int monitor_exec(taskStruct *task){
    //sprintf(printString, "monitor_exec => debut\n"); Serial.print(printString);    
    //int cardispo = Serial.available();
    int carlu;
    while(Serial.available() > 0) // tant qu'il y a des caractères à lire
    {
        carlu = Serial.read(); // on lit le caractère
        //sprintf(printString, "monitor_exec => car lu = %c\n",carlu); Serial.print(printString); 
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
        //sprintf(printString, "monitor_wait => caractere saisi\n"); Serial.print(printString);
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