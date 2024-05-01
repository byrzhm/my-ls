#include <dirent.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
  char name[NAME_MAX];
  int is_dir;
} File;

static int cmp_file(const void *a, const void *b)
{
  File *fa = (File *)a;
  File *fb = (File *)b;
  if (fa->is_dir == fb->is_dir) {
    return strcmp(fa->name, fb->name);
  } else {
    return fa->is_dir - fb->is_dir;
  }
}

typedef int ListFunc(const char *, const struct stat *, int);

static ListFunc list;
static int pathwalk(char *, ListFunc *);
static int walk_r(ListFunc *);
static int walk(ListFunc *);

#define RECURSIVE 0x01
#define ALL 0x02
#define LOWER 0x04
#define UPPER 0x08
#define MODIFIED 0x10

static int first_reg     = 1;
static int first_dir     = 1;
static int flags         = 0;
static int upper_bound   = 0;
static int lower_bound   = 0;
static time_t time_bound = 0;

int main(int argc, char *argv[])
{
  int opt;
  File *files;

  while ((opt = getopt(argc, argv, "ral:h:m:")) != -1) {
    switch (opt) {
      case 'r':
        flags |= RECURSIVE;
        break;

      case 'a':
        flags |= ALL;
        break;

      case 'l':
        flags       |= LOWER;
        lower_bound  = atoi(optarg);
        break;

      case 'h':
        flags       |= UPPER;
        upper_bound  = atoi(optarg);
        break;

      case 'm':
        flags      |= MODIFIED;
        time_bound  = time(NULL) - atoi(optarg) * 24 * 60 * 60;
        break;

      default:
        fprintf(stderr, "Usage: %s [-r] [-a] [-l lower_bound] [-h upper_bound] [-m time_bound] [path ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  if (optind == argc) {
    pathwalk(".", list);
  } else {
    files = malloc((argc - optind) * sizeof(File));
    for (int i = optind; i < argc; ++i) {
      struct stat statbuf;
      if (lstat(argv[i], &statbuf) < 0) {
        perror("lstat error");
        continue;
      }

      if (S_ISDIR(statbuf.st_mode)) {
        strcpy(files[i - optind].name, argv[i]);
        files[i - optind].is_dir = 1;
      } else {
        strcpy(files[i - optind].name, argv[i]);
        files[i - optind].is_dir = 0;
      }
    }
    qsort(files, argc - optind, sizeof(File), cmp_file);

    for (int i = 0; i < argc - optind; ++i) {
      if (pathwalk(files[i].name, list) != 0)
        exit(EXIT_FAILURE);
    }

    free(files);
  }
  printf("\n");

  exit(0);
}

static char *fullpath;
static size_t pathlen;
static int start;

static int pathwalk(char *pathname, ListFunc *func)
{
  int ret;
  fullpath = malloc(pathlen = 1024);

  if (pathlen <= strlen(pathname)) {
    pathlen = strlen(pathname) * 2;
    if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
      perror("realloc error");
      exit(EXIT_FAILURE);
    }
  }

  strcpy(fullpath, pathname);
  start = 0;
  if (flags & RECURSIVE) {
    ret = walk_r(func);
  } else {
    ret = walk(func);
  }
  free(fullpath);
  return ret;
}

#define WALK_F 1
#define WALK_D 2
#define WALK_DNR 3
#define WALK_NS 4

static int walk(ListFunc *func)
{
  struct stat statbuf;
  DIR *dp;
  struct dirent *dirp;
  int ret, n, save;

  if (lstat(fullpath, &statbuf) < 0) {
    return func(fullpath, &statbuf, WALK_NS);
  }

  if (S_ISDIR(statbuf.st_mode) == 0) {
    return func(fullpath, &statbuf, WALK_F);
  }

  n = strlen(fullpath);
  if (n + NAME_MAX + 2 > pathlen) {
    pathlen *= 2;
    if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
      perror("realloc error");
      exit(EXIT_FAILURE);
    }
  }

  if (first_dir || first_reg) {
    printf("\033[0m%s:\n", fullpath);
    first_dir = 0;
    first_reg = 0;
  } else {
    printf("\n\n\033[0m%s:\n", fullpath);
  }

  fullpath[n++] = '/';
  fullpath[n]   = 0;  // append a slash to the path
  save          = start;
  start         = n;

  if ((dp = opendir(fullpath)) == NULL) {  // can't read directory
    return func(fullpath, &statbuf, WALK_DNR);
  }

  while ((dirp = readdir(dp)) != NULL) {
    if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) {
      continue;  // ignore dot and dot-dot
    }

    if ((flags & ALL) == 0) {
      if (dirp->d_name[0] == '.') {
        continue;
      }
    }

    strcpy(&fullpath[n], dirp->d_name);
    if (lstat(fullpath, &statbuf) < 0) {
      return func(fullpath, &statbuf, WALK_NS);
    }

    if (S_ISDIR(statbuf.st_mode) == 0) {
      func(fullpath, &statbuf, WALK_F);
      continue;
    }

    if ((ret = func(fullpath, &statbuf, WALK_D)) != 0) {
      return ret;
    }
  }

  fullpath[n - 1] = 0;  // erase everything from slash onwards

  if (closedir(dp) < 0) {
    perror("closedir error");
    exit(EXIT_FAILURE);
  }

  start = save;

  return 0;
}

static int walk_r(ListFunc *func)
{
  struct stat statbuf;
  DIR *dp;
  struct dirent *dirp;
  int n;

  walk(func);

  n = strlen(fullpath);
  if (n + NAME_MAX + 2 > pathlen) {
    pathlen *= 2;
    if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
      perror("realloc error");
      exit(EXIT_FAILURE);
    }
  }

  fullpath[n++] = '/';
  fullpath[n]   = 0;

  if ((dp = opendir(fullpath)) == NULL) {
    return func(fullpath, NULL, WALK_DNR);
  }

  while ((dirp = readdir(dp)) != NULL) {
    if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) {
      continue;
    }

    if ((flags & ALL) == 0) {
      if (dirp->d_name[0] == '.') {
        continue;
      }
    }

    strcpy(&fullpath[n], dirp->d_name);
    if (lstat(fullpath, &statbuf) < 0) {
      return func(fullpath, &statbuf, WALK_NS);
    }

    if (S_ISDIR(statbuf.st_mode) && walk_r(func) != 0) {
      return -1;
    }
  }

  fullpath[n - 1] = 0;

  if (closedir(dp) < 0) {
    perror("closedir error");
    exit(EXIT_FAILURE);
  }

  return 0;
}

static int list(const char *pathname, const struct stat *statbuf, int type)
{
  const char *name     = &pathname[start];
  const char *filename = strrchr(pathname, '/');
  if (filename != NULL) {
    filename = filename + 1;
  } else {
    filename = name;
  }

  switch (type) {
    case WALK_F:
      switch (statbuf->st_mode & S_IFMT) {
        case S_IFREG:
          // filesize lower bound
          if (flags & LOWER && statbuf->st_size < lower_bound) {
            return 0;
          }
          // filesize upper bound
          if (flags & UPPER && statbuf->st_size > upper_bound) {
            return 0;
          }
          // file modified time
          if (flags & MODIFIED && statbuf->st_mtime < time_bound) {
            return 0;
          }

          if ((flags & ALL) == 0 && filename[0] == '.') {
            return 0;
          }

          if (first_reg) {
            first_reg = 0;
          }
          if (access(pathname, X_OK) == 0) {
            printf("\033[1;32m%s ", name);
          } else {
            printf("\033[0m%s ", name);
          }
          break;
      }
      break;

    case WALK_D:
      if ((flags & ALL) == 0 && filename[0] == '.') {
        return 0;
      }
      printf("\033[1;34m%s ", name);
      break;

    case WALK_DNR:
      fprintf(stderr, "can't read directory %s\n", pathname);
      break;

    case WALK_NS:
      fprintf(stderr, "stat error for %s\n", pathname);
      break;

    default:
      fprintf(stderr, "unknown type %d for pathname %s\n", type, pathname);
  }
  return 0;
}
