#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <getopt.h>

static void do_grep(regex_t *pat, FILE *f, int inv);
static void help(char *cmd, int status);

static struct option longopts[] = {
    {"ignore-case", no_argument, NULL, 'i'},
    {"invert-match", no_argument, NULL, 'v'},
    {0, 0, 0, 0}};

int main(int argc, char *argv[])
{
    regex_t pat;
    int i, opt, err;
    int r = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;
    int inv = 0; // false

    while ((opt = getopt_long(argc, argv, "ivh", longopts, NULL)) != -1)
    {
        switch (opt)
        {
        case 'i':
            r += REG_ICASE;
            break;
        case 'v':
            inv = 1; // true
            break;
        case 'h':
            help(argv[0], 0);
        case '?':
            help(argv[0], 1);
        }
    }

    if (argc - optind < 1)
    {
        fputs("no pattern\n", stderr);
        help(argv[0], 1);
    }

    err = regcomp(&pat, argv[optind], r);
    if (err != 0)
    {
        char buf[1024];

        regerror(err, &pat, buf, sizeof buf);
        puts(buf);
        exit(1);
    }

    if (argc - optind == 1)
    {
        do_grep(&pat, stdin, inv);
    }
    else
    {
        for (i = 1; i < argc - optind; i++)
        {
            FILE *f;
            f = fopen(argv[optind + i], "r");
            if (!f)
            {
                perror(argv[optind + i]);
                exit(1);
            }
            do_grep(&pat, f, inv);
            fclose(f);
        }
    }
    regfree(&pat);
    exit(0);
}

static void do_grep(regex_t *pat, FILE *src, int inv)
{
    char buf[4096];

    while (fgets(buf, sizeof buf, src))
    {
        if (regexec(pat, buf, 0, NULL, 0) == inv)
        {
            fputs(buf, stdout);
        }
    }
}

static void help(char *prg, int status)
{
    fprintf(stderr, "Usage: %s [-i | --ignore-case] [-v | --invert-match] PATTERN [FILE]...\n", prg);
    exit(status);
}
