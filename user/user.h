struct stat;
#include "rtcdate.h"
struct pinfo {
  int pid;
  int ppid;
  int state;
  uint64 sz;
  char name[16];
};
int getptable(int nproc, uint64 buffer);
// system calls
int fork(void);
int exit(int) __attribute__((noreturn));
int wait(int*);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(const char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int kbdint(void);/*** */
int countsyscall(void);
int getppid(void);
int datetime(struct rtcdate *);
int shutdown(void);
int rand(void);
int setsched(int);
int setpriority(int);
int fcfs(void);
int priority_sched(void);
int default_sched(void);

struct sched_metrics {
  int nproc;
  uint64 total_turnaround;
  uint64 total_wait;
};

int getmetrics(struct sched_metrics*);





// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void fprintf(int, const char*, ...) __attribute__ ((format (printf, 2, 3)));
void printf(const char*, ...) __attribute__ ((format (printf, 1, 2)));
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
int atoi(const char*);
int memcmp(const void *, const void *, uint);
void *memcpy(void *, const void *, uint);

// umalloc.c
void* malloc(uint);
void free(void*);
