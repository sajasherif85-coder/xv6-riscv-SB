#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int
main(int argc, char *argv[])
{

   int fact=1;

//int num = argc -1;
int num=atoi(argv[1]);

  if (argc < 2) {
        printf("Format: fact <number>\n");
        exit(1);
  }


 if(argv[1][0] == '-'){
 printf("no negative numbers allowed");
}
 else {
   for( int i = num; i>0 ; i-- )
    { fact=i*fact;
    }
    printf("the factorial= %d \n",fact);

 }

  exit(0);
}
