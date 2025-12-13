#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "format: mv source destination\n");
    exit(1);
  }

  char *src = argv[1];
  char *dst = argv[2];

  // Check if source file exists
  struct stat st;
  if(stat(src, &st) < 0){
    fprintf(2, "mv: cannot access '%s'\n", src);
    exit(1);
  }

  // Create a hard link to the source at the destination
  //make dst* point th src* by the inode and checck the return value if it increased
  if(link(src, dst) < 0){
    fprintf(2, "mv: cannot link '%s' to '%s'\n", src, dst);
    exit(1);
  }

  // Remove the original file
  //now theres is two link one link should be removed if link count = 1 then ni linking happed
  if(unlink(src) < 0){
    fprintf(2, "mv: cannot unlink '%s'\n", src);
    // Note: destination already linked, but source wasn't removed
    exit(1);
  }

  exit(0);
}
