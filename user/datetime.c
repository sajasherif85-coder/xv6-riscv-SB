#include "kernel/types.h"
#include "user/user.h"
#include "rtcdate.h"

int
main()
{
    struct rtcdate r;

    if(datetime(&r) < 0){
        printf("datetime syscall failed\n");
        exit(1);
    }

    printf("%d-%d-%d %d:%d:%d\n",
        r.year, r.month, r.day,
        r.hour, r.minute, r.second);

    exit(0);
}
