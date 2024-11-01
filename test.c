#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  char *s = strdup("hello\nword");
  char *d = "\n\r";
  char *result = NULL;

  result = strtok(s, d);
  printf("%s", result);
  while (result != NULL) {
    result = strtok(NULL, d);
    printf("%s", result);
  }
}
