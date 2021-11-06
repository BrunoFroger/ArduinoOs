//----------------------------------
//
//      tools.hpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>
#include "variables.hpp"

#ifndef __TOOLS_HPP
    #define __TOOLS_HPP

    extern char *tools_string_cut(char *inputString, char delimiter, int index);
    extern char *get_time(void);
    
#endif
