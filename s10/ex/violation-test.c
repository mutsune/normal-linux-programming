#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd;

    fd = open("hoge", O_RDWR);

    // rename before close
    if (rename("hoge", "moge") < 0)
    {
        perror("rename: hoge");
        exit(1);
    }

    // unlink before close
    // if (unlink("hoge") < 0)
    // {
    //     perror("unlink: hoge");
    //     exit(1);
    // }

    // rename another file
    // if (rename("foo", "bar") < 0)
    // {
    //     perror("rename: foo");
    //     exit(1);
    // }

    printf("close");
    close(fd);

    exit(0);
}
