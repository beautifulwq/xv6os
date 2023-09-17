#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"

int main(int argc, const char* argv) {
    int p[2];
    pipe(p);
    if (fork() == 0) {
        char x = 'c';
        if (read(p[0], &x, 1) != 1) exit(-1);
        fprintf(1, "%d: received ping\n", getpid());
        if (write(p[1], &x, 1) != 1) exit(-1);
        close(p[1]);
        exit(0);
    }
    else {
        char x = 'p';
        if (write(p[1], &x, 1) != 1) exit(-1);
        close(p[1]);
        wait(0);
        if (read(p[0], &x, 1) != 1) exit(-1);
        fprintf(1, "%d: received pong\n", getpid());
    }
    exit(0);
}
