//----------------------------------
//
//      ps.hpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>
#include "variables.hpp"

#ifndef __PS_HPP
    #define __PS_HPP

    extern void ps_init(taskStructPtr);
    extern int ps_exec(taskStructPtr);
    extern void ps_wait(taskStructPtr);
    extern void ps_wakeup(taskStructPtr);

#endif
