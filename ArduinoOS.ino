//----------------------------------
//
//      ArduinoOS.ino
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>
#include "variables.hpp"
#include "task.hpp"
#include "scheduler.hpp"
#include "log.hpp"


void setup(){
    // initialize serial communication
    Serial.begin(115200);
    int timeoutInitSerial = 1000;
    while (timeoutInitSerial-- > 0)
    {
        if (Serial)
            break;
        delay(1);
    }
    delay(1000);
    Serial.println("");
    Serial.print("Serial initialized in ");
    Serial.print(1000 - timeoutInitSerial);
    Serial.println(" ms");

    task_init();
    //log("task_init => ok\n");
    // run the monitor
    //log("lancement des taches de base : sdcard et monitor\n");
    task_add("sdcard", 50, -1);
    //log("sdcard => ok\n");
    task_add("monitor", 50, -1);
    //log("monitor => ok\n");
    //task_add("afficheur", 50, -1);
    // run tasks for testing
    if (false){
        int nb_iterations = 10;
        task_add("nop", 55, -1);
        task_add("nop", 63, -1);
        task_add("nop", 62, -1);
        //log("ps %d",nb_iterations);
        //task_add(printString, standardPriority, -1);
        log("env %d",nb_iterations);
        //task_add(printString, standardPriority, -1);
    }
    log("lancement du scheduler\n");
    scheduler_init();
    log("scheduler_init => ok\n");
}

//-----------------------------------
//
//      loop
//
//-----------------------------------
void loop(){
    scheduler_loop();
    //task_list();
    delay(10);
    //delay(1000);
    //Serial.print(".");
}