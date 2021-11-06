//----------------------------------
//
//      afficheur.hpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>

#ifndef __AFFICHEUR_HPP
    #define __AFFICHEUR_HPP

    extern void afficheur_init(taskStructPtr);
    extern int afficheur_exec(taskStructPtr);
    extern void afficheur_wait(taskStructPtr);
    extern void afficheur_wakeup(taskStructPtr);

#endif