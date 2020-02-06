#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

#define MAX_RESTRICT 1

static void do_grep(regex_t *pat, FILE *f);
static void help(char *cmd, int status);

int main(int argc, char *argv[])
{
    regex_t pat;
    int i, err;

    if (argc < 2)
    {
        fputs("no pattern\n", stderr);
        help(argv[0], 1);
    }

    err = regcomp(&pat, argv[1], 0);
    if (err != 0)
    {
        char buf[1024];

        regerror(err, &pat, buf, sizeof buf);
        puts(buf);
        exit(1);
    }

    if (argc == 2)
    {
        do_grep(&pat, stdin);
    }
    else
    {
        for (i = 2; i < argc; i++)
        {
            FILE *f;
            f = fopen(argv[i], "r");
            if (!f)
            {
                perror(argv[i]);
                exit(1);
            }
            do_grep(&pat, f);
            fclose(f);
        }
    }
    regfree(&pat);
    exit(0);
}

static void do_grep(regex_t *pat, FILE *src)
{
    char buf[4096];
    regmatch_t match[MAX_RESTRICT];
    int start, end, len;

    while (fgets(buf, sizeof buf, src))
    {
        if (regexec(pat, buf, MAX_RESTRICT, match, 0) == 0)
        {
            for (int i = 0; i < MAX_RESTRICT; i++)
            {
                start = match[i].rm_so;
                end = match[i].rm_eo;
                len = end - start;
                for (int j = start; j < end; j++)
                {
                    putchar(buf[j]);
                }
            }
            putchar('\n');
        }
    }
}

static void help(char *prg, int status)
{
    fprintf(stderr, "Usage: %s PATTERN [FILE]...\n", prg);
    exit(status);
}
