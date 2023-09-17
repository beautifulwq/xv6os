#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void check(int p[]) {
    int prime;
    int n;
    close(p[1]);
    if (read(p[0], &prime, 4) != 4) {
        exit(-1);
    }

    fprintf(1, "prime %d\n", prime);

    int newp[2];
    pipe(newp);
    if (fork() == 0) {
        check(newp);
    }
    else {
        close(newp[0]);
        while (read(p[0], &n, 4) == 4)
            if (n % prime != 0)
                write(newp[1], &n, 4);
        close(p[0]);
        close(newp[1]);//don't forget this line
        wait(0);
    }

    exit(0);
}

int main(int argc, char argv[]) {
    int p[2];
    pipe(p);
    if (fork() == 0) {
        check(p);
    }
    else {
        close(p[0]);
        for (int i = 2;i < 37;i++)
            write(p[1], &i, 4);
        close(p[1]);
        wait(0);
        exit(0);
    }
    exit(0);
}
