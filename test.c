#include <grp.h>
#include <stdio.h>
#include <sys/stat.h>

int main() {
  char *path = "/home/ht/code/Command/..";
  struct stat file;

  stat(path, &file);

  for (int i = 0; i < 10; i++) {
    struct group *grp = getgrgid(1000);
    printf("%s\n", grp->gr_name);
  }
}
