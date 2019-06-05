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

#define BUFFER_SIZE 2048

static void do_cat(int fd, const char *path)
{
  unsigned char buf[BUFFER_SIZE];
  int n;

  if (fd < 0)
  {
    die(path);
  }

  for (;;)
  {
    n = read(fd, buf, sizeof buf);
    if (n < 0)
    {
      die(path);
    }
    if (n == 0)
    {
      break;
    }
    if (write(STDOUT_FILENO, buf, n) < 0)
    {
      die(path);
    }
  }
  if (close(fd) < 0)
  {
    die(path);
  }
}

static void die(const char *s)
{
  perror(s);
  exit(1);
}
