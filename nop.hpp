//----------------------------------
//
//      not.hpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>
#include "variables.hpp"

#ifndef __NOP_HPP
    #define __NOP_HPP

    extern void nop_init(taskStructPtr);
    extern int nop_exec(taskStructPtr);
    extern void nop_wait(taskStructPtr);
    extern void nop_wakeup(taskStructPtr);

#endif
