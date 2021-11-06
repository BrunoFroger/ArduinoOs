//----------------------------------
//
//      unset.hpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>
#include "variables.hpp"

#ifndef __UNSET_HPP
    #define __UNSET_HPP

    extern void unset_init(taskStructPtr);
    extern int unset_exec(taskStructPtr);
    extern void unset_wait(taskStructPtr);

#endif
