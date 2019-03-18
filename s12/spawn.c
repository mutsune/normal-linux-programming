#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <command> <arg>\n", argv[0]);
        exit(1);
    }
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork(2) failed\n");
        exit(1);
    }
    if (pid == 0) { // 子プロセス
        execl(argv[1], argv[1], argv[2], NULL);
        // ここには来ない想定。来たら失敗
        perror(argv[1]);
        exit(99);
    } else { // 親プロセス
        int status;

        // wait せずに親プロセスが死ぬと、子プロセスは init の子、ゾンビプロセスとなる
        sleep(10);
        exit(0);

        waitpid(pid, &status, 0);
        printf("child (PID=%d) finished; ", pid);
        if (WIFEXITED(status)) {
            printf("exit, status=%d\n", WEXITSTATUS(status));
        } else if (WIFEXITED(status)) {
            printf("signal, sig=%d\n", WTERMSIG(status));
        } else {
            printf("abnormal exit\n");
        }
        exit(0);
    }
}
