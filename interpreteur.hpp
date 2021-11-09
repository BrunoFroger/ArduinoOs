//----------------------------------
//
//      interpreteur.hpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------

#include <arduino.h>
#include "variables.hpp"

#ifndef __INTERPRETEUR_HPP
    #define __INTERPRETEUR_HPP

    //extern void interpreteur(taskStructPtr task);
    extern void interpreteur_init(taskStructPtr);
    extern int interpreteur_exec(taskStructPtr);
    extern void interpreteur_wait(taskStructPtr);
    extern void interpreteur_wakeup(taskStructPtr);
#endif