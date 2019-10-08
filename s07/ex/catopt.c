#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define _GNU_SOURCE
#include <getopt.h>

static void do_cat_file(const char *path, int v);
static void do_cat(int fd, const char *path, int v);
static void cat(FILE *f, const char *path);
static void visual_cat(FILE *f, const char *path);
static void die(const char *);

static struct option longopts[] = {
    {"visual", required_argument, NULL, 'v'},
    {0, 0, 0, 0}};

int main(int argc, char *argv[])
{
  int i, opt;

  int v = 0; // false

  while ((opt = getopt_long(argc, argv, "v", longopts, NULL)) != -1)
  {
    switch (opt)
    {
    case 'v':
      v = 1; // true
      break;
    case 'h':
      fprintf(stdout, "Usage: %s [-v] [FILE ...]\n", argv[0]);
      exit(0);
    case '?':
      fprintf(stdout, "Usage: %s [-v] [FILE ...]\n", argv[0]);
      exit(1);
    }
  }

  if (argc == optind)
  {
    do_cat(STDIN_FILENO, "stdin", v);
  }
  else
  {
    for (i = optind; i < argc; i++)
    {
      do_cat_file(argv[i], v);
    }
  }
  exit(0);
}

static void do_cat_file(const char *path, int v)
{
  int fd;

  fd = open(path, O_RDONLY);
  do_cat(fd, path, v);
}

static void do_cat(int fd, const char *path, int v)
{
  FILE *f;

  f = fdopen(fd, "rb");
  if (!f)
  {
    die(path);
  }

  if (v)
  {
    visual_cat(f, path);
  }
  else
  {
    cat(f, path);
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

#define BUFFER_SIZE 4096

static void cat(FILE *f, const char *path)
{
  unsigned char buf[BUFFER_SIZE];
  size_t size;
  int n;

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
}

static void visual_cat(FILE *f, const char *path)
{
  int c, e;

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
      die(path);
    }
  }
}

static void die(const char *s)
{
  perror(s);
  exit(1);
}
