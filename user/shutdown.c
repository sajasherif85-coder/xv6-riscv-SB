#include "kernel/types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  //mapped in memlayout
  // map in vm.c

    printf("Calling shutdown syscall\n");
    shutdown();   // calls kernel via usys wrapper
    // if shutdown returns, exit
    exit(0);
}
