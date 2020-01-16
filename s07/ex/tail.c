#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

#define MAX_LINE_LENGTH 65536

static void do_tail(FILE *f, long nlines);
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
      perror(argv[i]);
      exit(1);
    }

    do_tail(f, nlines);
    fclose(f);
  }

  exit(0);
}

static void do_tail(FILE *f, long nlines)
{
  char buffer[nlines][MAX_LINE_LENGTH];
  int i = 0;
  int j = 0;
  long p = 0;
  long q = 0;
  char c = 0;

  if (nlines <= 0)
  {
    return;
  }

  while ((c = getc(f)) != EOF)
  {
    // printf("%d, %ld, %d = [%c]\n\n", i, p, j, c);
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

  // printf("p: %d\n", p);
  // printf("max: %ld\n", q);
  for (i = p; i < q; i++)
  {
    p = i % nlines;
    // printf("p, i: %ld, %ld\n", p, i - 1);
    for (j = 0; j < MAX_LINE_LENGTH && (c = buffer[p][j]) != '\n' && c != '\0'; j++)
    {
      // printf("i, p, j -> c: %d, %ld, %d -> %c\n", i, p, j, c);
      if ((int)c == EOF)
      {
        // printf("eof\n");
        return;
      }

      if (putchar(c) < 0)
      {
        exit(1);
      }
    }
    putchar('\n');
    // printf("i < q: %d < %ld\n", i, q);
  }
}

static void help(char *cmd, int status)
{
  fprintf(stderr, "Usage: %s [-n NUM | --lines=NUM] [FILE]...\n", cmd);
  exit(status);
}
