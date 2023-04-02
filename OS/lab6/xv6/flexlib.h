#include "memlayout.h"
#include <stdbool.h>

int offset = 0;

#define pgentry __int32_t

struct FlexEntry {
    int syscallnum;
    int argcount;
    bool status;
    int args[6];
    int returnval;
};
typedef struct FlexEntry flex_e;


int make_entry(flex_e *entry_struct){
    flex_e* pgptr = (flex_e *) P2V(PHYSTOP-0x1000 + offset);
    offset += sizeof(flex_e);
    *pgptr = *entry_struct;

    return 0;
}



int pgwrite(int fd, const void* outstr, int sz){
    //write syscall number : 16
    //no. of args = 3
    //status - initially zero (indicates the process is yet to be executed); one if executed
    //args - fd, outstr, sz
    //return value

    flex_e sysentry;
    sysentry.syscallnum = 16;
    sysentry.argcount = 3;
    sysentry.status = false;
    sysentry.args[0] = fd;
    sysentry.args[1] = (int) outstr;
    sysentry.args[2] = sz;

    make_entry(&sysentry);
    return 1;
}

// #define SYS_fork    1
// #define SYS_exit    2
// #define SYS_wait    3
// #define SYS_pipe    4
// #define SYS_read    5
// #define SYS_kill    6
// #define SYS_exec    7
// #define SYS_fstat   8
// #define SYS_chdir   9
// #define SYS_dup    10
// #define SYS_getpid 11
// #define SYS_sbrk   12
// #define SYS_sleep  13
// #define SYS_uptime 14
// #define SYS_open   15
// #define SYS_write  16
// #define SYS_mknod  17
// #define SYS_unlink 18
// #define SYS_link   19
// #define SYS_mkdir  20
// #define SYS_close  21
// #define SYS_fregister 22

int pgfork(){
    flex_e forksysentry;
    forksysentry.syscallnum = 1;
    forksysentry.argcount = 0;
    forksysentry.status = false;
    
    make_entry(&forksysentry);
    return 0;
}

int pgread(int fd, void *buf, int count){

    flex_e sysentry;
    sysentry.syscallnum = 5;
    sysentry.argcount = 3;
    sysentry.status = false;
    sysentry.args[0] = fd;
    sysentry.args[1] = (int) buf;
    sysentry.args[2] = count;

    make_entry(&sysentry);
    return 1;
}



