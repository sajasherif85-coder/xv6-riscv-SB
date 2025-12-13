#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[512];

void
wcfile(int fd, int *l, int *w, int *c, int *L)
{
    int n, i;
    int inword = 0;
    int curlen = 0;

    *l = *w = *c = *L = 0;

    while((n = read(fd, buf, sizeof(buf))) > 0){
        for(i = 0; i < n; i++){
            (*c)++;
            curlen++;

            if(buf[i] == '\n'){
                (*l)++;
                if(curlen - 1 > *L)
                    *L = curlen - 1;
                curlen = 0;
            }

            if(strchr(" \t\r\n\v", buf[i]))
                inword = 0;
            else if(!inword){
                (*w)++;
                inword = 1;
            }
        }
    }

    if(curlen > *L)
        *L = curlen;
}

void
print_usage()
{
    printf("usage: wc [-lwcL] [file...]\n");
}

int
main(int argc, char *argv[])
{
    int show_l = 0, show_w = 0, show_c = 0, show_L = 0;
    int l, w, c, L;
    int tl = 0, tw = 0, tc = 0, tL = 0;
    int i, fd;
    int first_file = 1;
    int file_count = 0;



    if (argc ==1) {
        print_usage();
        exit(1);
    }
    if(argc > 1 && argv[1][0] == '-'){
        char *p = argv[1] + 1;

        if(*p == 0){  // case: "-"
            printf("wc: invalid option -- '-'\n");
            print_usage();
            exit(1);
        }

        while(*p){
            if(*p == 'l') show_l = 1;
            else if(*p == 'w') show_w = 1;
            else if(*p == 'c') show_c = 1;
            else if(*p == 'L') show_L = 1;

            else {
                // INVALID FLAG
                printf("wc: invalid option -- %c\n", *p);
                print_usage();
                exit(1);
            }
            p++;
        }
        first_file = 2; // place in argv where files start
    }


    // default (no flags)
    if(!show_l && !show_w && !show_c && !show_L)
        show_l = show_w = show_c = 1;

    // read from stdin
    if(first_file >= argc){
        wcfile(0, &l, &w, &c, &L);

        if(show_l) printf("%d ", l);
        if(show_w) printf("%d ", w);
        if(show_c) printf("%d ", c);
        if(show_L) printf("%d ", L);
        printf("\n");
        exit(0);
    }

    // multiple files
    for(i = first_file; i < argc; i++){
        if((fd = open(argv[i], O_RDONLY)) < 0){
            printf("wc: cannot open %s\n", argv[i]);
            exit(1);
        }

        wcfile(fd, &l, &w, &c, &L);
        close(fd);

        if(show_l) printf("%d ", l);
        if(show_w) printf("%d ", w);
        if(show_c) printf("%d ", c);
        if(show_L) printf("%d ", L);
        printf("%s\n", argv[i]);

        tl += l;
        tw += w;
        tc += c;
        if(L > tL) tL = L;

        file_count++;
    }

    // totals
    if(file_count > 1){
        if(show_l) printf("%d ", tl);
        if(show_w) printf("%d ", tw);
        if(show_c) printf("%d ", tc);
        if(show_L) printf("%d ", tL);
        printf("total\n");
    }

    exit(0);
}
