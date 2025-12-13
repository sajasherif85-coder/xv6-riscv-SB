#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"


int main(int argc, char *argv[]) {

  int pid;
  int k, nprocess = 10;
  int z;
  char buffer_src[1024], buffer_dst[1024];
  int sched_type = 1; // 0=RR, 1=FCFS, 2=Priority

  if(argc > 1){
    sched_type = argv[1][0] - '0';
  }

  if(setsched(sched_type) < 0){
    printf("Failed to set scheduler\n");
    exit(1);
  }

  printf("Running with scheduler %d\n", sched_type);

  for (k = 0; k < nprocess; k++) {
    // ensure different creation times (proc->ctime)
    // needed for properly testing FCFS scheduling
    sleep(2);

    pid = fork();
    if (pid < 0) {
      printf("%d failed in fork!\n", getpid());
      exit(0);

    }
    else if (pid == 0) {
      // child
      printf("[pid=%d] created\n", getpid());

      if(sched_type == 2){
        // set priority based on k, lower k higher priority
        setpriority(k);
      }

      // Do work in bursts to allow preemption
      for(int burst = 0; burst < 10; burst++) {
        for (z = 0; z < 100000; z += 1) {
           // copy buffers one inside the other and back
           // used for wasting cpu time
           memmove(buffer_dst, buffer_src, 1024);
           memmove(buffer_src, buffer_dst, 1024);
        }
        sleep(1);  // Allow other processes to run
      }
      exit(0);
    }
  }

  for (k = 0; k < nprocess; k++) {
    pid = wait(0);
    printf("[pid=%d] terminated\n", pid);
  }

  struct sched_metrics m[3];
  if(getmetrics(m) == 0){
    printf("Metrics for scheduler %d:\n", sched_type);
    if(m[sched_type].nproc > 0){
      printf("  Processes: %d\n", m[sched_type].nproc);
      printf("  Average Turnaround Time: %ld ticks\n", m[sched_type].total_turnaround / m[sched_type].nproc);
      printf("  Average Waiting Time: %ld ticks\n", m[sched_type].total_wait / m[sched_type].nproc);
    } else {
      printf("  No processes completed under this scheduler.\n");
    }
  } else {
    printf("Failed to get metrics\n");
  }

  exit(0);
}
