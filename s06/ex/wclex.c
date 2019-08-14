#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int i;

  for (i = 1; i < argc; i++)
  {
    FILE *f;
    int c, n;
    char p;

    f = fopen(argv[i], "r");
    if (!f)
    {
      perror(argv[i]);
      exit(1);
    }
    n = 0;
    while ((c = fgetc(f)) != EOF)
    {
      p = c;
      if (c == '\n')
      {
        n++;
      }
    }
    if (p != '\n')
    {
      n++;
    }

    printf("%d", n);
    fclose(f);
  }
  exit(0);
}
