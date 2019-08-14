#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int i;

  for (i = 1; i < argc; i++)
  {
    FILE *f;
    int c, e;

    f = fopen(argv[i], "r");
    if (!f)
    {
      perror(argv[i]);
      exit(1);
    }
    while ((c = fgetc(f)) != EOF)
    {
      e = 0;
      if (c == '\t')
      {
        printf("\\t");
      }
      else
      {
        if (c == '\n')
        {
          putchar('$');
        }
        e = putchar(c);
      }

      if (e < 0)
      {
        exit(1);
      }
    }
    fclose(f);
  }
  exit(0);
}
