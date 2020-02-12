#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

#define MAX_PATH_LEGTH 256

void traverse(char *path);

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
        traverse(argv[i]);
    }
    exit(0);
}

void traverse(char *path)
{
    char child[MAX_PATH_LEGTH];
    DIR *d;
    struct dirent *ent;
    char *name;

    d = opendir(path);
    if (!d)
    {
        perror(path);
        exit(1);
    }

    printf("%s\n", path);

    while ((ent = readdir(d)))
    {
        name = ent->d_name;

        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
        {
            continue;
        }

        if (ent->d_type == DT_DIR)
        {
            sprintf(child, "%s/%s", path, name);
            traverse(child);
        }
        else
        {
            printf("%s/%s\n", path, name);
        }
    }

    closedir(d);
}
