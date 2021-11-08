//----------------------------------
//
//      sdcard.hpp
//
//----------------------------------
//
// Copyright B.Froger (2021)
//
//----------------------------------


#include <arduino.h>
#include "variables.hpp"
#include "task.hpp"

#ifndef __SDCARD_HPP
    #define __SDCARD_HPP

    extern void sdcard_init(taskStructPtr);
    extern int sdcard_exec(taskStructPtr);
    extern void sdcard_wait(taskStructPtr);
    extern void sdcard_wakeup(taskStructPtr);
    char *sdcard_pwd(taskStruct *task);
    bool is_sdcard_initialized(void);

#endif
