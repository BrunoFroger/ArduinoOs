//----------------------------------
//
//      variables.cpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------

#include <arduino.h>
#include "variables.hpp"

char printString[250];

taskStruct taskTbl[NB_TASKS];
//char internalCommands[NB_INTERNAL_COMMANDS][SIZE_INTERNAL_COMMAND];
int standardPriority=DEFAULT_PRIORITY;
int currentPriority=standardPriority;
int newPid=1;
int nb_process;
int run_process;
int wait_process;
int active_process;
