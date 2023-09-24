#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(2, "xargs:too few argv\n");
        exit(-1);
    }
    if (argc - 1 >= MAXARG) {
        printf("xargs : maxium amount of args is %d !\n", MAXARG);
        exit(1);
    }
    char* new_argv[MAXARG];
    for (int i = 1;i < argc;i++)
        new_argv[i - 1] = argv[i];

    int index = argc - 1;

    char buf[512];
    int is_line_end = 0;
    int id = 0;
    while (read(0, &buf[id], sizeof(char))) {
        if (buf[id] == '\n') is_line_end = 1;
        if (buf[id] == ' ' || buf[id] == '\n') {
            buf[id] = 0;
            new_argv[index++] = buf;
            id = 0;
        }


        id++;//正常往下读入字符，不和上边id=0连起来看

        if (is_line_end == 1) {
            is_line_end = 0;
            new_argv[index] = 0;
            index = argc - 1;
            int pid = fork();
            if (pid == 0) {
                exec(new_argv[0], new_argv);
            }
            else if (pid < 0) exit(-1);
            else wait(( int* ) 0);
        }
    }

    exit(0);
}
