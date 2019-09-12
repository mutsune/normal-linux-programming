#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void do_cat_file(const char *path);
static void do_cat(int fd, const char *path);
static void die(const char *);

int main(int argc, char *argv[])
{
  int i;

  if (argc == 1)
  {
    do_cat(STDIN_FILENO, "stdin");
  }
  else
  {
    for (i = 1; i < argc; i++)
    {
      do_cat_file(argv[i]);
    }
  }
  exit(0);
}

static void do_cat_file(const char *path)
{
  int fd;

  fd = open(path, O_RDONLY);
  do_cat(fd, path);
}

#define BUFFER_SIZE 4096

static void do_cat(int fd, const char *path)
{
  FILE *f;
  unsigned char buf[BUFFER_SIZE];
  size_t size;
  int n;

  f = fdopen(fd, "rb");
  if (!f)
  {
    die(path);
  }

  size = sizeof(unsigned char);

  while (0 != (n = fread(buf, size, BUFFER_SIZE, f)))
  {
    if (n == EOF)
    {
      break;
    }
    if (ferror(f))
    {
      die(path);
    }

    if (fwrite(buf, size, n, stdout) < n)
    {
      die(path);
    }
  }

  if (fflush(stdout) == EOF)
  {
    die(path);
  }

  if (fclose(f) == EOF)
  {
    die(path);
  }
}

static void die(const char *s)
{
  perror(s);
  exit(1);
}
