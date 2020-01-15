#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_LINE_LENGTH 65536

static void do_tail(FILE *f, long nlines);

int main(int argc, char *argv[])
{
  int i;
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
  // long k = 0;
  // long n = 0;

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
  // exit(0);

  // c = getc(f);
  // printf("%d", (int)c);
  // exit(1);

  // while ((int)(c = (char)getc(f)) != EOF)
  // {
  //   if (c == '\n')
  //   {
  //     i++;
  //     j = 0;
  //   }
  //   buffer[i % nlines][j++] = c;
  // }

  // for (n = 0; n < nlines; n++)
  // {
  //   j = 0;
  //   k = i < nlines ? 0 : (i + 1) % nlines;
  //   if (buffer[k][j] == '\0')
  //   {
  //     return;
  //   }
  //   while (putchar(buffer[k][j]) > 0)
  //   {
  //     j++;
  //   }
  // }

  // for (i = 0; i < sizeof(buffer) / sizeof(buffer[0]); i++)
  // {
  //   for (j = 0; j < sizeof(buffer[i]) / sizeof(buffer[i][0]); j++)
  //   {
  //     if (buffer[i][j] == '\0')
  //     {
  //       return;
  //     }

  //     if (putchar(buffer[i][j]) < 0)
  //     {
  //       exit(1);
  //     }
  //   }
  // }

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
