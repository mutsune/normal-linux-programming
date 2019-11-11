#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static void do_tail(FILE *f, long nlines);
static long get_next_line(long last, long nlines);

int main(int argc, char *argv[])
{
  int i, c;
FILE *f;
  long nlines = 2;

  if (argc < 1)
  {
    fprintf(stderr, "Usage: %s [file file...]\n", argv[0]);
    exit(1);
  }

  if (argc == 1)
  {
    do_tail(stdin, nlines);
    exit(0);
  }

  for (i = 1; i < argc; i++)
  {
    FILE *f;

    f = fopen(argv[i], "r");
    if (!f)
    {
      perror(argv[i]);
      exit(1);
    }
    printf("i: %d", i);
    do_tail(f, nlines);
    fclose(f);
  }

  exit(0);
}

static void do_tail(FILE *f, long nlines)
{
  char buffer[nlines][INT_MAX];
  long i = 0;
  int c, j = 0;

  if (nlines <= 0)
  {
    return;
  }

  c = getc(f);
  printf("c: %c", c);
  exit(0);

  for (i = nlines - 1; i >= 0 && c != EOF; i--) {
    j = 0;
    while ((c = getc(f)) != EOF || c != '\n')
    {
      buffer[i][j++] = c;
    }
  }

  for (i = 0; i < sizeof(buffer) / sizeof(buffer[0]); i++)
  {
    for (j = 0; j < sizeof(buffer[i]) / sizeof(buffer[i][0]); j++)
    {
      if (buffer[i][j] == '\0') {
        return;
      }

      if (putchar(buffer[i][j]) < 0)
      {
        exit(1);
      }
    }
  }
}
