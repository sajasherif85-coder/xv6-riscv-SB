#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MAX_LINE 512

// Helper to read one line from fd into buf
// Returns number of chars read (excluding newline), or -1 on error, or 0 on EOF
int
readline(int fd, char *buf, int maxlen)
{
  int n;
  int len = 0;

  while(len < maxlen - 1){
    if((n = read(fd, buf + len, 1)) <= 0){
      return (n < 0) ? -1 : len;  // Error or EOF
    }
    if(buf[len] == '\n'){
      buf[len] = '\0';
      return len;
    }
    len++;
  }

  // Line too long
  buf[len] = '\0';
  return len;
}

int
main(int argc, char *argv[])
{
  int fd1, fd2;
  char line1[MAX_LINE], line2[MAX_LINE];
  int n1, n2;
  int line_num1 = 1, line_num2 = 1;
  int differences = 0;

  if(argc != 3){
    fprintf(2, "usage: diff <file1> <file2>\n");
    exit(1);
  }

  // Open both files
  if((fd1 = open(argv[1], O_RDONLY)) < 0){
    fprintf(2, "diff: cannot open %s\n", argv[1]);
    exit(1);
  }
  if((fd2 = open(argv[2], O_RDONLY)) < 0){
    fprintf(2, "diff: cannot open %s\n", argv[2]);
    close(fd1);
    exit(1);
  }

  // Read and compare lines
  while(1){
    n1 = readline(fd1, line1, MAX_LINE);
    n2 = readline(fd2, line2, MAX_LINE);

    if(n1 < 0 || n2 < 0){
      fprintf(2, "diff: read error\n");
      close(fd1);
      close(fd2);
      exit(1);
    }

    // Both files exhausted
    if(n1 == 0 && n2 == 0){
      break;
    }

    // Only file1 has content left
    if(n1 > 0 && n2 == 0){
      printf("Line %d only in %s:\n", line_num1, argv[1]);
      printf("< %s\n", line1);
      differences++;
      line_num1++;
      continue;
    }

    // Only file2 has content left
    if(n1 == 0 && n2 > 0){
      printf("Line %d only in %s:\n", line_num2, argv[2]);
      printf("> %s\n", line2);
      differences++;
      line_num2++;
      continue;
    }

    // Both have lines - compare
    if(strcmp(line1, line2) != 0){
      printf("Line %d differs:\n", line_num1);
      printf("< %s\n", line1);
      printf("> %s\n", line2);
      differences++;
    }

    line_num1++;
    line_num2++;
  }

  close(fd1);
  close(fd2);

  // Report result
  if(differences == 0){
    printf("Files are identical\n");
  }

  exit(0);
}
