#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  struct pinfo table[64];

  if (!getptable(64, (uint64)table)) {
    printf("ps: getptable failed\n");
    exit(1);
  }

  printf("PID\tPPID\tSTATE\tSIZE\tNAME\n");

  for (int i = 0; i < 64; i++) {
    if (table[i].pid == 0)
      break;

    printf("%d\t%d\t%d\t%lu\t%s\n",
      table[i].pid,
      table[i].ppid,
      table[i].state,
      table[i].sz,
      table[i].name);
  }

  exit(0);
}
