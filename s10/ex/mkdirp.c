#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

void mkdir_validate(char *path)
{
    if (mkdir(path, 0777) < 0)
    {
        perror(path);
        exit(1);
    }
}

void mkdirp(char *org)
{
    int i;
    char c;
    char path[strlen(org)];

    i = 0;

    while ((c = org[i]) != '\0')
    {
        path[i] = c;
        path[i + 1] = '\0';
        // printf("path: %s\n", path);

        if (c == '/' && !opendir(path))
        {
            mkdir_validate(path);
        }
        i++;
    }
    mkdir_validate(path);
}

int main(int argc, char *argv[])
{
    int i;

    if (argc < 2)
    {
        fprintf(stderr, "%s: no arguments\n", argv[0]);
        exit(1);
    }

    for (i = 1; i < argc; i++)
    {
        char *org = argv[i];
        mkdirp(org);
    }
    exit(0);
}
