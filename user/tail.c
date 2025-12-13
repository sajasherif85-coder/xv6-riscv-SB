// tail: print the last N lines of a file or stdin
// Usage: tail [-n NUM] [file]

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "user/user.h"

static int
is_digits(const char *s)
{
  if(!s || *s == '\0')
    return 0;
  for(;  *s != '\0' ; s++){
    if(*s < '0' || *s > '9')
      return 0;
  }
  return 1;
}

int
main(int argc, char *argv[])
{
  int fd = 0; // default stdin
  int N = 10; // number of lines to print
  int i = 1, r; //i to know where is the file name in the argv, r for read return value
  static char data[65536]; // 64KB buffer for reading file
  int cap = sizeof(data);
  int total = 0;
  char tmp[512];

  // -n NUM or -nNUM
  if(argc >= 2 && argv[1][0] == '-' && argv[1][1] == 'n')
  {
    if(argv[1][2] != '\0')
    {
      // -nNUM
      if(!is_digits(&argv[1][2])){
        fprintf(2, "tail: invalid number after -n\n");
        exit(1);
      }
      N = atoi(&argv[1][2]);
      i = 2;
    }

    else
    {
      // -n NUM
      if(argc < 3)
      {
        fprintf(2, "tail: missing number after -n\n");
        exit(1);
      }
      if(!is_digits(argv[2])){
        fprintf(2, "tail: invalid number after -n\n");
        exit(1);
      }
      N = atoi(argv[2]);
      i = 3;
    }


   /* if(N <= 0){
      fprintf(2, "tail: number of lines must be positive\n");
      exit(1);
    }*/

  }

  if(i < argc)
  {
    if((fd = open(argv[i], O_RDONLY)) < 0)
    {
      fprintf(2, "tail: cannot open %s\n", argv[i]);
      exit(1);
    }
  }
   else
  {
    fprintf(2, "tail: missing file argument\n");
    exit(1);
  }


  // read whole file into fixed-size buffer (max file size: MAXFILE * BSIZE)
  while((r = read(fd, tmp, sizeof(tmp))) > 0)
  {
    if(total + r > cap){
      fprintf(2, "tail: file too large\n");
      exit(1);
    }
    memmove(data + total, tmp, r);
    total += r;
  }
  if(r < 0){
    fprintf(2, "tail: read error\n");
    exit(1);
  }

  // count back N newlines; start from end
  if(total == 0){
    // nothing to print
    exit(0);
  }
  int idx = total - 1;
  int lines = 0;
  // If the file ends with a newline, last line is after it, so we count newlines until N
  while(idx >= 0 && lines < N){
    if(data[idx] == '\n')
      lines++;
    idx--;
  }
  int start = idx + 1;
  // if we couldn't find enough newlines, start remains 0

  // write from start to total
  if(write(1, data + start, total - start) != total - start){
    fprintf(2, "tail: write error\n");
    exit(1);
  }

  if(fd != 0)
    close(fd);

  exit(0);
}
