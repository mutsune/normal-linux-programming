#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

#define LEN_WIN 5

static void do_tail(FILE *f, long nlines);
static void fbuffer(char *buffer[], int nlines);
static void die(const char *s);
static void help(char *cmd, int status);

static struct option longopts[] = {
    {"lines", required_argument, NULL, 'n'},
    {0, 0, 0, 0}};

int main(int argc, char *argv[])
{
    int i, opt;
    FILE *f;
    long nlines = 10;

    while ((opt = getopt_long(argc, argv, "n:h", longopts, NULL)) != -1)
    {
        switch (opt)
        {
        case 'n':
            errno = 0;
            nlines = strtol(optarg, NULL, 10);
            if (errno == ERANGE || errno == EINVAL)
            {
                fprintf(stderr, "invalid argument: %s\n", optarg);
                help(argv[0], 1);
            }
            break;
        case 'h':
            help(argv[0], 0);
        case '?':
            help(argv[0], 1);
        }
    }

    if (argc == optind)
    {
        do_tail(stdin, nlines);
        exit(0);
    }

    for (i = optind; i < argc; i++)
    {
        f = fopen(argv[i], "r");

        if (!f)
        {
            die(argv[1]);
        }

        do_tail(f, nlines);
        fclose(f);
    }

    exit(0);
}

static void do_tail(FILE *f, long nlines)
{
    char *buffer[nlines];
    long maxl[nlines];
    long i = 0;
    long j = 0;
    long p = 0;
    long q = 0;
    char c = 0;

    if (nlines <= 0)
    {
        return;
    }

    while ((c = getc(f)) != EOF)
    {
        if (j == 0)
        {
            buffer[p] = (char *)calloc(LEN_WIN, sizeof(char));
            maxl[p] = LEN_WIN;
        }
        // printf("j = %d, maxl = %d\n", j, maxl);
        if (j + 1 == maxl[p])
        {
            char *ext = (char *)realloc(buffer[p], sizeof(char) * (maxl[p] + LEN_WIN));
            if (ext == NULL)
            {
                fbuffer(buffer, nlines);
                die("Cannot reallocate memory");
            }
            buffer[p] = ext;
            ext = NULL;
            memset(buffer[p] + maxl[p] + 1, '\0', sizeof(char) * LEN_WIN);
            maxl[p] += LEN_WIN;
        }

        buffer[p][j++] = c;

        if (c == '\n')
        {
            j = 0;
            p = ++i % nlines;
        }
    }

    if (i < nlines)
    {
        p = 0;
        q = i;
    }
    else
    {
        q = p + nlines;
    }

    // printf("p: %ld\n", p);
    // printf("max: %ld\n", q);
    for (i = p; i < q; i++)
    {
        p = i % nlines;
        // printf("p, i, maxl[p]: %ld, %ld, %ld\n", p, i - 1, maxl[p]);
        for (j = 0; j < maxl[p] && (c = buffer[p][j]) != '\n' && c != '\0'; j++)
        {
            // printf("i, p, j -> c: %d, %ld, %d -> %c\n", i, p, j, c);
            if ((int)c == EOF)
            {
                // printf("eof\n");
                return;
            }

            if (putchar(c) < 0)
            {
                fbuffer(buffer, nlines);
                perror("Unexpected error");
                exit(1);
            }
        }
        putchar('\n');
        // printf("i < q: %d < %ld\n", i, q);
    }

    fbuffer(buffer, nlines);
}

static void fbuffer(char *buffer[], int nlines)
{
    int i;
    for (i = 0; i < nlines; i++)
    {
        free(buffer[i]);
    }
}

static void die(const char *s)
{
    perror(s);
    exit(1);
}

static void help(char *cmd, int status)
{
    fprintf(stderr, "Usage: %s [-n NUM | --lines=NUM] [FILE]...\n", cmd);
    exit(status);
}
