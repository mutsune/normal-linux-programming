#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

static void do_wcl_file(const char *path);
static void do_wcl(int fd, const char *path);
static void writeChar(unsigned char c);
static void writeInt(unsigned int c);
static void die(const char *);

int main(int argc, char *argv[])
{
  int i;

  if (argc == 1)
  {
    do_wcl(STDIN_FILENO, "stdin");
  }
  else
  {
    for (i = 1; i < argc; i++)
    {
      do_wcl_file(argv[i]);
    }
  }
  exit(0);
}

static void do_wcl_file(const char *path)
{
  int fd;

  fd = open(path, O_RDONLY);
  do_wcl(fd, path);
}

#define BUFFER_SIZE 1

static void do_wcl(int fd, const char *path)
{
  unsigned char buf[BUFFER_SIZE];
  unsigned int c;
  int result;

  if (fd < 0)
  {
    die(path);
  }

  // init
  c = 0;

  for (;;)
  {
    result = read(fd, buf, 1);
    if (result < 0)
    {
      die(path);
    }
    if (result == 0)
    {
      break;
    }

    // count newline
    if (buf[0] == '\n')
    {
      c++;
    }
  }

  writeInt(c);
  writeChar('\n');

  if (close(fd) < 0)
  {
    die(path);
  }
}

static void writeChar(unsigned char c)
{
  if (write(STDOUT_FILENO, &c, 1) < 0)
  {
    die("unknown char");
  }
}

static void writeInt(unsigned int c)
{
  if (c < 10)
  {
    writeChar(c + '0');
  }
  else
  {
    writeInt(c / 10);
    writeChar(c % 10 + '0');
  }
}

static void die(const char *s)
{
  perror(s);
  exit(1);
}
