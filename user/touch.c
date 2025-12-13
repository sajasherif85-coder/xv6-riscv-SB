#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
  int i, file;

  if(argc == 1)
  {
    fprintf(2, "rewrite the file name\n");
    exit(1);
  }

  for(i = 1; i < argc ; i++)
  {
    // If it already exists -> error
    if((file = open(argv[i], O_RDONLY)) >= 0)
    {
      fprintf(2, "touch: %s already exists \n", argv[i]);
      close(file);
      continue;
    }

    // create the file
    if((file = open(argv[i], O_CREATE|O_WRONLY)) < 0)
    {
      fprintf(2, "touch: cannot create %s\n", argv[i]);
      exit(1);
    }
    close(file);
  }

  exit(0);
}
