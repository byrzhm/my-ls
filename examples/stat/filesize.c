#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
  int i;
  struct stat buf;

  for (i = 1; i < argc; ++i) {
    printf("%s: ", argv[i]);
    if (lstat(argv[i], &buf) < 0) {
      perror("lstat error");
      continue;
    }

    if (S_ISREG(buf.st_mode)) {
      printf("%s, size: %ld\n", argv[i], buf.st_size);
    }
  }

  exit(0);
}
