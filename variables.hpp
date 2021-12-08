//----------------------------------
//
//      variables.hpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------

#include <arduino.h>
//#include <FS.h>
#include <SD.h>

#ifndef __VARIABLES_HPP
    #define __VARIABLES_HPP

    #if defined(__AVR__)
        #define SD_CS    10
        #define BOARD   "AVR"
    #elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        #define SD_CS     53
        #define BOARD   "ATMega"
    #elif defined(ESP8266)
        #define SD_CS    D8
        #define BOARD   "ESP8266"
    #elif defined(ESP32)
    //#define SD_CS    5   //SS
    #   define SD_CS    14
        #define BOARD   "ESP32"
    #else
        #define BOARD   "inconnu"
    #endif 


    #include "task.hpp"

    // define tacsk variables
    #define NB_TASKS    100
    #define INIT 8
    #define FREE 9
    #define RUN 1
    #define WAIT 2
    #define DEAD 3

    #define TASK_RETURN_OK  0
    #define TASK_RETURN_KO  1
    #define DEFAULT_PRIORITY    64
    #define NB_VAR_PAR_TACHE    10

    //extern char printString[250];

    extern int standardPriority;
    extern int currentPriority;
    extern int newPid;
    extern int nb_process;      // nb total of process
    extern int run_process;     // process who wait for running
    extern int wait_process;    // process waiting an evenement
    extern int active_process;  // process with lower priority than running

    typedef struct taskStructure *taskStructPtr;

    //typedef struct contextStructure *contextStructurePtr;

    typedef struct variableStructure{
        char key[10];
        int value;
    } t_variable;

    typedef struct sdCardParamStruct {
        bool recursif;
        File currentDirectory;
        char currentDirectoryName[50];
        char currentPathName[50];
        File sdcardDataFile;
    } t_SdCardParam;

    typedef struct contextStructure {
        char fileName[50];
        File ptrFile;
        int lineNumber;
        int nbErreurs=0;
        bool insideIf=false;
        bool resultTest;
        bool insideElse=false;
    } t_context;

    typedef struct taskStructure{
        char name[50];
        int pid;
        int ppid;
        int priority;
        int currentPriority;
        int status;
        void (*initFct)(taskStructPtr task);
        int (*execFct)(taskStructPtr task);
        void (*waitFct)(taskStructPtr task);
        void (*wakeupFct)(taskStructPtr task);
        t_variable lstVar[NB_VAR_PAR_TACHE];

        t_context context;
        t_SdCardParam sdCardParam;

    } taskStruct; 

    extern taskStruct taskTbl[];

#endif