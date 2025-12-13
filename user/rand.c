#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{

  printf("Testing rand syscall:\n");

    int r = rand();
    printf("rand() = %d\n", r);

  printf("rand test completed\n");
  exit(0);
}
