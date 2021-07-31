#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define LINE_BUF_SIZE 4096
#define ARG_BUF_SIZE 1024
#define ARG_LEN 1024

void init_cmd(char *cmd[ARG_LEN]);
char *readline(char buf[LINE_BUF_SIZE], int buf_size);
int parse(char buf[LINE_BUF_SIZE], char file[ARG_BUF_SIZE], char *cmd[ARG_LEN]);
void exec(char file[ARG_BUF_SIZE], char *cmd[ARG_LEN]);

int main()
{
    char buf[LINE_BUF_SIZE];
    char file[ARG_BUF_SIZE];
    char *cmd[ARG_LEN];

    while (readline(buf, sizeof buf))
    {
        init_cmd(cmd);

        if (parse(buf, file, cmd) == 1)
        {
            continue;
        }
        exec(file, cmd);
    }

    exit(0);
}

void init_cmd(char *cmd[ARG_LEN])
{
    char cmd_buf[ARG_BUF_SIZE][ARG_LEN];
    int i;

    for (i = 0; i < ARG_LEN; i++)
    {
        cmd[i] = cmd_buf[i];
    }
}

char *readline(char buf[LINE_BUF_SIZE], int buf_size)
{
    printf("> ");
    return fgets(buf, buf_size, stdin);
}

int parse(char buf[LINE_BUF_SIZE], char file[ARG_BUF_SIZE], char *cmd[ARG_LEN])
{
    char c;
    int n = 0;
    int i = 0;
    int j = 0;
    int offset = 0;
    int prev_is_space = 1;

    // skip head spaces
    while ((c = buf[n]) == ' ')
    {
        n++;
    }
    offset = n;

    // parse file
    while (1)
    {
        if ((c = buf[n++]) == ' ' || c == '\n' || c == '\0' || c == EOF)
        {
            break;
        }
        file[j++] = c;
    }
    file[j] = '\0';

    // parse cmd
    n = offset;
    j = 0;
    while (1)
    {
        // printf("n: %d, i: %d, j: %d\n", n, i, j);
        if ((c = buf[n++]) == '\n' || c == '\0' || c == EOF)
        {
            if (!prev_is_space)
            {
                // printf("insert \\0 into (i, j) = (%d, %d) because this is end\n", i, j);
                cmd[i++][j] = '\0';
            }
            break;
        }
        else if (c == ' ')
        {
            if (!prev_is_space)
            {
                // printf("insert \\0 into (i, j) = (%d, %d) because this is space\n", i, j);
                cmd[i++][j] = '\0';
                j = 0;
            }
            prev_is_space = 1;
        }
        else
        {
            cmd[i][j++] = c;
            // printf("inserted cmd[%d][%d]: %c\n", i, j - 1, cmd[i][j - 1]);
            prev_is_space = 0;
        }
    }

    cmd[i++] = NULL;

    return i;
}

void exec(char file[ARG_BUF_SIZE], char *cmd[ARG_LEN])
{
    pid_t pid;

    // int i = 0;
    // printf("file: [%s]\n", file);
    // for (i = 0; cmd[i]; i++)
    // {
    //     printf("cmd[%d]: [%s]\n", i, cmd[i]);
    // }

    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "execution failed\n");
        exit(1);
    }
    if (pid == 0)
    { // child process
        execvp(file, cmd);

        perror(file);
        exit(99);
    }
    else
    { // parent process
        int status;

        waitpid(pid, &status, 0);
        printf("child (PID=%d) finished; ", pid);
        if (WIFEXITED(status))
        {
            printf("exit, status=%d\n", WEXITSTATUS(status));
        }
        else if (WIFEXITED(status))
        {
            printf("signal, sig=%d\n", WTERMSIG(status));
        }
        else
        {
            printf("abnormal exit\n");
        }
    }
}
