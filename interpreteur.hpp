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

    extern void interpreteur(taskStructPtr task, t_context *context);
#endif