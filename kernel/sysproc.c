#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

extern struct proc proc[NPROC];
extern int sched_mode;

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_getppid(void)
{
  struct proc *p = myproc(); //myproc gets the current process
  if (p->parent)
    return p->parent->pid;
  else
    return 0; // No parent process
}

uint64
sys_getptable(void)
{
  int nproc;
  uint64 buffer;

  argint(0, &nproc);
  argaddr(1, &buffer);

  return getptable(nproc, buffer);
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}
/*
// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
*/
uint64
sys_setsched(void)
{
  int mode;
  argint(0, &mode);
  if(mode < 0 || mode > 2) return -1;
  sched_mode = mode;
  return 0;
}

uint64
sys_setpriority(void)
{
  int pri;
  argint(0, &pri);
  myproc()->priority = pri;
  return 0;
}

uint64
sys_getmetrics(void)
{
  uint64 addr;
  argaddr(0, &addr);
  struct sched_metrics *m = metrics;
  if(copyout(myproc()->pagetable, addr, (char*)m, sizeof(metrics)) < 0)
    return -1;
  return 0;
}
/*uint64 sys_set_fcfs(void) {
  sched_mode = SCHED_FCFS;
  return 0;
}

uint64 sys_set_priority_sched(void) {
  sched_mode = SCHED_PRIORITY;
  return 0;
}

uint64 sys_set_default_sched(void) {
  sched_mode = SCHED_ROUND_ROBIN;
  return 0;
}*/
