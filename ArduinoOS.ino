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
    // run the monitor
    task_add("sdcard", 50, -1);
    task_add("monitor", 50, -1);
    //task_add("afficheur", 50, -1);
    // run tasks for testing
    if (false){
        int nb_iterations = 10;
        task_add("nop", 55, -1);
        task_add("nop", 63, -1);
        task_add("nop", 62, -1);
        //sprintf(printString,"ps %d",nb_iterations);
        //task_add(printString, standardPriority, -1);
        sprintf(printString,"env %d",nb_iterations);
        //task_add(printString, standardPriority, -1);
    }
    scheduler_init();
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