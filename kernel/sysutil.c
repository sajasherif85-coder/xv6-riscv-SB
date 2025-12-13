#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "datetime.h"
#include "bootepoch.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#define VIRT_TEST 0x100000

// Convert seconds since 1970 → struct rtcdate
void unix_to_rtc(uint64 ts, struct rtcdate *r) {
    uint64 sec = ts % 60;
    ts /= 60;
    uint64 min = ts % 60;
    ts /= 60;
    uint64 hr = ts % 24;
    ts /= 24;  // ts now = days since 1970-01-01

    int year = 1970;
    while (1) {
        int days = (year%4==0 && year%100!=0) || (year%400==0) ? 366 : 365;
        if (ts < days) break;
        ts -= days;
        year++;
    }
    static int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int month = 0;
    while (1) {
        int d = mdays[month];
        if (month==1 && ((year%4==0 && year%100!=0) || year%400==0)) d=29; // leap year
        if (ts < d) break;
        ts -= d;
        month++;
    }

    r->year = year;
    r->month = month + 1;
    r->day = ts + 1;
    r->hour = hr ;
    r->minute = min;
    r->second = sec;
}

extern uint ticks;  // kernel timer ticks

uint64 sys_datetime(void) {
    uint64 addr;
    struct rtcdate r;

    // get user-space pointer
    argaddr(0, &addr);

    // use kernel's ticks counter (incremented by timer interrupt)
    // xv6 runs at ~10 ticks per second, so divide by 10 to get seconds
    uint64 seconds = ticks / 10;

    // convert to calendar date
    unix_to_rtc(BOOT_EPOCH + seconds, &r);

    // copy to user space
    if (either_copyout(1, addr, (char*)&r, sizeof(r)) < 0)
        return -1;

    return 0;
}

extern int kbd_intr_count;
uint64 sys_kbdint(void) {
    return kbd_intr_count;
}

extern uint64 syscall_count;
uint64 sys_countsyscall(void) {
    return syscall_count;
}


uint64
sys_shutdown(void)
{
    printf("Shutting down xv6...\n");

    volatile uint32 *fin = (volatile uint32*)VIRT_TEST;

    *fin = 0x5555;   // QEMU exits with “pass” status

    // Should never return
    for(;;);
}


// PRNG state
static uint64 rand_state = 1;

// Simple LCG: X_{n+1} = (a * X_n + c) mod m
// Using values from Numerical Recipes
uint64
sys_rand(void)
{
  rand_state = rand_state * 1664525 + 1013904223;
  return rand_state;
}

// Initialize PRNG with current time
void
rand_init(void)
{
  rand_state = r_time();  // Seed with hardware timer
}


// return how many clock tick interrupts have occurred
// since start.

uint64 sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
