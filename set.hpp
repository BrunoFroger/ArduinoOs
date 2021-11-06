//----------------------------------
//
//      set.hpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>
#include "variables.hpp"

#ifndef __SET_HPP
    #define __SET_HPP

    extern void set_init(taskStructPtr);
    extern int set_exec(taskStructPtr);
    extern void set_wait(taskStructPtr);
    extern void set_wakeup(taskStructPtr);

#endif
