//----------------------------------
//
//      kill.hpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>
#include "variables.hpp"

#ifndef __KILL_HPP
    #define __KILL_HPP

    extern void kill_init(taskStructPtr);
    extern int kill_exec(taskStructPtr);
    extern void kill_wait(taskStructPtr);
    extern void kill_wakeup(taskStructPtr);

#endif
