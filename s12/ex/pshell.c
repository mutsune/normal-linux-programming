#include <stdio.h>
#include <stdlib.h>

#define LINE_BUF_SIZE 4096

char *readline(char buf[LINE_BUF_SIZE], int buf_size);
int exec(char buf[LINE_BUF_SIZE]);

int main()
{
    char buf[LINE_BUF_SIZE];

    while (readline(buf, sizeof buf))
    {
        exec(buf);
    }

    exit(0);
}

char *readline(char buf[LINE_BUF_SIZE], int buf_size)
{
    printf("> ");
    return fgets(buf, buf_size, stdin);
}

int exec(char buf[LINE_BUF_SIZE])
{
    FILE *fd;
    char c;

    if ((fd = popen(buf, "r")) == NULL)
    {
        perror("Failed to exec");
        return EXIT_FAILURE;
    }

    while ((c = fgetc(fd)) != EOF && c != '\0')
    {
        printf("%c", c);
    }

    pclose(fd);

    return EXIT_SUCCESS;
}
