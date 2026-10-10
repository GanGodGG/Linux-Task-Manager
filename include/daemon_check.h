#ifndef DMONCHECK_H
#define DMONCHECK_H
#include <stdio.h>
#include <dirent.h> // Directory entry library for reading directories in linux
#include "vector.h"
#include "misc.h"
// The daemon check file for ... CHECKING the daemons, actually.
enum token_value{
    NAME = 1,
    STATE = 3,
    PID = 6,
    PPID = 7,
    VMPEAK = 17,
    VMSIZE = 18
};
struct daemon {         // the daemon struct 
    char *name;         // -- the name is actual DAEMON name: stored in the /proc/<PID>/comm file.
    char *description;  // -- the description is a short description of the daemon.
    char *pid;            // -- pid of the daemon.
    char *is_running;     // -- 1 if the daemon is running, 0 otherwise
    
};

Vector* get_all_daemons(); // return a vector of struct daemon.
#endif