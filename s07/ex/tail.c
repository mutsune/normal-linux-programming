#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_LINE_LENGTH 65536

static void do_tail(FILE *f, long nlines);

typedef struct ring_buffer
{
  int i;
  int nlines;
  char **lines;
} ring_buffer;

void push(ring_buffer *b, int n, char *line)
{
  // b->lines[(n + (*b).i++) % (*b).nlines] = line;
  b->lines[n] = line;
}

int main(int argc, char *argv[])
{
  int nlines = 2;
  char init[nlines][MAX_LINE_LENGTH];
  char line[MAX_LINE_LENGTH];

  ring_buffer b = {0,
                   nlines,
                   init};

  for (int i = 0; i < MAX_LINE_LENGTH; i++)
  {
    line[i] = 'a';
  }
  push(&b, 0, line);

  for (int i = 0; i < MAX_LINE_LENGTH; i++)
  {
    line[i] = 'b';
  }

  push(&b, 1, line);

  for (int i = 0; i < MAX_LINE_LENGTH; i++)
  {
    line[i] = 'c';
  }
  push(&b, 2, line);

  printf("%d\n", b.i);
  printf("%c\n", b.lines[0][0]);
  printf("%c\n", b.lines[1][0]);

  exit(0);
}

// int main(int argc, char *argv[])
// {
//   int i;
//   FILE *f;
//   long nlines = 2;

//   if (argc < 1)
//   {
//     fprintf(stderr, "Usage: %s [file file...]\n", argv[0]);
//     exit(1);
//   }

//   if (argc == 1)
//   {
//     do_tail(stdin, nlines);
//     exit(0);
//   }

//   for (i = 1; i < argc; i++)
//   {
//     int ch;
//     f = fopen(argv[i], "r");
//     char *filename = argv[i];
//     f = fopen(filename, "r");
//     while ((ch = getc(f)) != EOF)
//     {
//       putchar(ch);
//     }

//     if (!f)
//     {
//       perror(argv[i]);
//       exit(1);
//     }

//     do_tail(f, nlines);
//     fclose(f);
//   }

//   exit(0);
// }

// static void do_head(FILE *f, long nlines) {
//   int c;

//   if (nlines <= 0) return;
//   while ((c = getc(f)) != EOF) {
//     if (putchar(c) < 0) exit(1);
//     if (c == '\n') {
//       nlines--;
//       if (nlines == 0) return;
//     }
//   }
// }

static void do_tail(FILE *f, long nlines)
{
  char buffer[nlines][INT_MAX];
  long i = 0;
  int j = 0;
  long k = 0;
  long n = 0;
  // char c = 0;
  int c = 0;

  // push(&b, line);

  if (nlines <= 0)
  {
    return;
  }

  while ((c = getc(f)) != EOF)
  {
    if (c == '\n')
    {
      i++;
      j = 0;
    }
    printf("%ld, %ld, %d\n", i, (i % nlines), j++);
    // putchar(c);
    buffer[i % nlines][j++] = c;
  }
  exit(0);

  c = getc(f);
  printf("%d", (int)c);
  // exit(1);

  while ((int)(c = (char)getc(f)) != EOF)
  {
    if (c == '\n')
    {
      i++;
      j = 0;
    }
    buffer[i % nlines][j++] = c;
  }

  for (n = 0; n < nlines; n++)
  {
    j = 0;
    k = i < nlines ? 0 : (i + 1) % nlines;
    if (buffer[k][j] == '\0')
    {
      return;
    }
    while (putchar(buffer[k][j]) > 0)
    {
      j++;
    }
  }

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
}
