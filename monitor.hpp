//----------------------------------
//
//      monitor.hpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>

#ifndef __MONITOR_HPP
    #define __MONITOR_HPP

    extern void monitor_init(taskStructPtr);
    extern int monitor_exec(taskStructPtr);
    extern void monitor_wait(taskStructPtr);
    extern void monitor_wakeup(taskStructPtr);

#endif
