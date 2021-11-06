//----------------------------------
//
//      priority.hpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>
#include "variables.hpp"

#ifndef __PRIORITY_HPP
    #define __PRIORITY_HPP

    extern void priority_init(taskStructPtr);
    extern int priority_exec(taskStructPtr);
    extern void priority_wait(taskStructPtr);
    extern void priority_wakeup(taskStructPtr);

#endif
