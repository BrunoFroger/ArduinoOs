//----------------------------------
//
//      env.hpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>
#include "variables.hpp"

#ifndef __ENV_HPP
    #define __ENV_HPP

    #define NB_ENV_VARS 100
    
    typedef struct envData {
        char key[50];
        char value[50];
    } envStruct;
    extern envStruct envTbl[];

    extern void env_init(taskStructPtr);
    extern int env_exec(taskStructPtr);
    extern void env_wait(taskStructPtr);
    extern void env_wakeup(taskStructPtr);

#endif
