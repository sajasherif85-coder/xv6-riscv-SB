#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int count = countsyscall();
  printf("Systems calls count :%d\n", count);
  exit(0);
}
