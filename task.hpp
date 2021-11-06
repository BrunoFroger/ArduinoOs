//----------------------------------
//
//      task.hpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------

#include <arduino.h>
#include "variables.hpp"

#ifndef __TASK_HPP
    #define __TASK_HPP
    
    extern void task_init(void);
    extern int task_add(char *name, int priority, int parentIdx);
    extern void task_reset(int indice);

#endif